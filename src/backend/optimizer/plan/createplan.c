/*-------------------------------------------------------------------------
 *
 * createplan.c--
 *    Routines to create the desired plan for processing a query
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/optimizer/plan/createplan.c,v 1.1.1.1 1996/07/09 06:21:37 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "c.h"

#include "nodes/execnodes.h"
#include "nodes/plannodes.h"
#include "nodes/relation.h"
#include "nodes/primnodes.h"
#include "nodes/nodeFuncs.h"

#include "nodes/makefuncs.h"

#include "utils/elog.h"
#include "utils/lsyscache.h"
#include "utils/palloc.h"
#include "utils/builtins.h"

#include "parser/parse_query.h"
#include "optimizer/clauseinfo.h"
#include "optimizer/clauses.h"
#include "optimizer/planmain.h"
#include "optimizer/tlist.h"
#include "optimizer/planner.h"
#include "optimizer/xfunc.h"
#include "optimizer/internal.h"


#define TEMP_SORT    1
#define TEMP_MATERIAL    2

static List *switch_outer(List *clauses);

static Scan *create_scan_node(Path *best_path, List *tlist);

static Join *create_join_node(JoinPath *best_path, List *tlist);

static SeqScan *create_seqscan_node(Path *best_path, List *tlist,
                                    List *scan_clauses);

static IndexScan *create_indexscan_node(IndexPath *best_path, List *tlist,
                                        List *scan_clauses);

static NestLoop *create_nestloop_node(JoinPath *best_path, List *tlist,
                                      List *clauses, Plan *outer_node, List *outer_tlist,
                                      Plan *inner_node, List *inner_tlist);

static MergeJoin *create_mergejoin_node(MergePath *best_path, List *tlist,
                                        List *clauses, Plan *outer_node, List *outer_tlist,
                                        Plan *inner_node, List *inner_tlist);

static HashJoin *create_hashjoin_node(HashPath *best_path, List *tlist,
                                      List *clauses, Plan *outer_node, List *outer_tlist,
                                      Plan *inner_node, List *inner_tlist);

static Node *fix_indxqual_references(Node *clause, Path *index_path);

static Temp *make_temp(List *tlist, List *keys, Oid *operators,
                       Plan *plan_node, int temptype);

static IndexScan *make_indexscan(List *qptlist, List *qpqual, Index scanrelid,
                                 List *indxid, List *indxqual);

static NestLoop *make_nestloop(List *qptlist, List *qpqual, Plan *lefttree,
                               Plan *righttree);

static HashJoin *make_hashjoin(List *tlist, List *qpqual,
                               List *hashclauses, Plan *lefttree, Plan *righttree);

static Hash *make_hash(List *tlist, Var *hashkey, Plan *lefttree);

static MergeJoin *make_mergesort(List *tlist, List *qpqual,
                                 List *mergeclauses, Oid opcode, Oid *rightorder,
                                 Oid *leftorder, Plan *righttree, Plan *lefttree);

static Material *make_material(List *tlist, Oid tempid, Plan *lefttree,
                               int keycount);

/*    
 * create_plan--
 *    Creates the access plan for a query by tracing backwards through the
 *    desired chain of pathnodes, starting at the node 'best-path'.  For
 *    every pathnode found:
 *    (1) Create a corresponding plan node containing appropriate id,
 *        target list, and qualification information.
 *    (2) Modify ALL clauses so that attributes are referenced using
 *        relative values.
 *    (3) Target lists are not modified, but will be in another routine.
 *    
 *    best-path is the best access path
 *
 *    Returns the optimal(?) access plan.
 */
Plan *
create_plan(Path *best_path) {
    List *tlist;
    Plan *plan_node = (Plan *) NULL;
    Rel *parent_rel;
    int size;
    int width;
    int pages;
    int tuples;

    parent_rel = best_path->parent;
    tlist = get_actual_tlist(parent_rel->targetlist);
    size = parent_rel->size;
    width = parent_rel->width;
    pages = parent_rel->pages;
    tuples = parent_rel->tuples;

    switch (best_path->pathtype) {
        case T_IndexScan :
        case T_SeqScan :
            plan_node = (Plan *) create_scan_node(best_path, tlist);
            break;
        case T_HashJoin :
        case T_MergeJoin :
        case T_NestLoop:
            plan_node = (Plan *) create_join_node((JoinPath *) best_path, tlist);
            break;
        default:
            /* do nothing */
            break;
    }

    plan_node->plan_size = size;
    plan_node->plan_width = width;
    if (pages == 0) pages = 1;
    plan_node->plan_tupperpage = tuples / pages;

#if 0 /* fix xfunc */
    /* sort clauses by cost/(1-selectivity) -- JMH 2/26/92 */
    if (XfuncMode != XFUNC_OFF)
    {
        set_qpqual((Plan) plan_node, 
               lisp_qsort( get_qpqual((Plan) plan_node), 
                  xfunc_clause_compare));
        if (XfuncMode != XFUNC_NOR)
        /* sort the disjuncts within each clause by cost -- JMH 3/4/92 */
        xfunc_disjunct_sort(plan_node->qpqual);
    }
#endif

    return (plan_node);
}

/*    
 * create_scan_node--
 *   Create a scan path for the parent relation of 'best-path'.
 *    
 *   tlist is the targetlist for the base relation scanned by 'best-path'
 *    
 *   Returns the scan node.
 */
static Scan *
create_scan_node(Path *best_path, List *tlist) {

    Scan *node;
    List *scan_clauses;

    /*
     * Extract the relevant clauses from the parent relation and replace the
     * operator OIDs with the corresponding regproc ids.
     *
     * now that local predicate clauses are copied into paths in
     * find_rel_paths() and then (possibly) pulled up in xfunc_trypullup(),
     * we get the relevant clauses from the path itself, not its parent
     * relation.   --- JMH, 6/15/92
     */
    scan_clauses = fix_opids(get_actual_clauses(best_path->locclauseinfo));

    switch (best_path->pathtype) {
        case T_SeqScan :
            node = (Scan *) create_seqscan_node(best_path, tlist, scan_clauses);
            break;

        case T_IndexScan:
            node = (Scan *) create_indexscan_node((IndexPath *) best_path,
                                                  tlist,
                                                  scan_clauses);
            break;

        default :
            elog(WARN, "create_scan_node: unknown node type",
                 best_path->pathtype);
            break;
    }

    return node;
}

/*    
 * create_join_node --
 *    Create a join path for 'best-path' and(recursively) paths for its
 *    inner and outer paths.
 *    
 *    'tlist' is the targetlist for the join relation corresponding to
 *    	'best-path'
 *    
 *    Returns the join node.
 */
static Join *
create_join_node(JoinPath *best_path, List *tlist) {
    Plan *outer_node;
    List *outer_tlist;
    Plan *inner_node;
    List *inner_tlist;
    List *clauses;
    Join *retval;

    outer_node = create_plan((Path *) best_path->outerjoinpath);
    outer_tlist = outer_node->targetlist;

    inner_node = create_plan((Path *) best_path->innerjoinpath);
    inner_tlist = inner_node->targetlist;

    clauses = get_actual_clauses(best_path->pathclauseinfo);

    switch (best_path->path.pathtype) {
        case T_MergeJoin:
            retval = (Join *) create_mergejoin_node((MergePath *) best_path,
                                                    tlist,
                                                    clauses,
                                                    outer_node,
                                                    outer_tlist,
                                                    inner_node,
                                                    inner_tlist);
            break;
        case T_HashJoin:
            retval = (Join *) create_hashjoin_node((HashPath *) best_path,
                                                   tlist,
                                                   clauses,
                                                   outer_node,
                                                   outer_tlist,
                                                   inner_node,
                                                   inner_tlist);
            break;
        case T_NestLoop:
            retval = (Join *) create_nestloop_node((JoinPath *) best_path,
                                                   tlist,
                                                   clauses,
                                                   outer_node,
                                                   outer_tlist,
                                                   inner_node,
                                                   inner_tlist);
            break;
        default:
            /* do nothing */
            elog(WARN, "create_join_node: unknown node type",
                 best_path->path.pathtype);
    }

#if 0
    /*
     ** Expensive function pullups may have pulled local predicates
     ** into this path node.  Put them in the qpqual of the plan node.
     **        -- JMH, 6/15/92
     */
    if (get_locclauseinfo(best_path) != NIL)
    set_qpqual((Plan)retval,
           nconc(get_qpqual((Plan) retval), 
             fix_opids(get_actual_clauses
                   (get_locclauseinfo(best_path)))));
#endif

    return (retval);
}

/*****************************************************************************
 *
 *  BASE-RELATION SCAN METHODS
 *
 *****************************************************************************/


/*    
 * create_seqscan_node--
 *   Returns a seqscan node for the base relation scanned by 'best-path'
 *   with restriction clauses 'scan-clauses' and targetlist 'tlist'.
 */
static SeqScan *
create_seqscan_node(Path *best_path, List *tlist, List *scan_clauses) {
    SeqScan *scan_node = (SeqScan *) NULL;
    Index scan_relid = -1;
    List *temp;

    temp = best_path->parent->relids;
    if (temp == NULL)
        elog(WARN, "scanrelid is empty");
    else
        scan_relid = (Index) lfirst(temp); /* ??? who takes care of lnext? - ay */
    scan_node = make_seqscan(tlist,
                             scan_clauses,
                             scan_relid,
                             (Plan *) NULL);

    scan_node->plan.cost = best_path->path_cost;

    return (scan_node);
}

/*    
 * create_indexscan_node--
 *    Returns a indexscan node for the base relation scanned by 'best-path'
 *    with restriction clauses 'scan-clauses' and targetlist 'tlist'.
 */
static IndexScan *
create_indexscan_node(IndexPath *best_path,
                      List *tlist,
                      List *scan_clauses) {
    /*
     * Extract the(first if conjunct, only if disjunct) clause from the
     * clauseinfo list.
     */
    Expr *index_clause = (Expr *) NULL;
    List *indxqual = NIL;
    List *qpqual = NIL;
    List *fixed_indxqual = NIL;
    IndexScan *scan_node = (IndexScan *) NULL;


    /*
     * If an 'or' clause is to be used with this index, the indxqual
     * field will contain a list of the 'or' clause arguments, e.g., the
     * clause(OR a b c) will generate: ((a) (b) (c)).  Otherwise, the
     * indxqual will simply contain one conjunctive qualification: ((a)).
     */
    if (best_path->indexqual != NULL)
        /* added call to fix_opids, JMH 6/23/92 */
        index_clause = (Expr *)
                lfirst(fix_opids(get_actual_clauses(best_path->indexqual)));

    if (or_clause((Node *) index_clause)) {
        List *temp = NIL;

        foreach(temp, index_clause->args) indxqual = lappend(indxqual, lcons(lfirst(temp), NIL));
    } else {
        indxqual = lcons(get_actual_clauses(best_path->indexqual),
                         NIL);
    }

    /*
     * The qpqual field contains all restrictions except the indxqual.
     */
    if (or_clause((Node *) index_clause))
        qpqual = set_difference(scan_clauses,
                                lcons(index_clause, NIL));
    else
        qpqual = set_difference(scan_clauses, lfirst(indxqual));

    fixed_indxqual =
            (List *) fix_indxqual_references((Node *) indxqual, (Path *) best_path);

    scan_node =
            make_indexscan(tlist,
                           qpqual,
                           lfirsti(best_path->path.parent->relids),
                           best_path->indexid,
                           fixed_indxqual);

    scan_node->scan.plan.cost = best_path->path.path_cost;

    return (scan_node);
}

/*****************************************************************************
 *
 *  JOIN METHODS
 *
 *****************************************************************************/

static NestLoop *
create_nestloop_node(JoinPath *best_path,
                     List *tlist,
                     List *clauses,
                     Plan *outer_node,
                     List *outer_tlist,
                     Plan *inner_node,
                     List *inner_tlist) {
    NestLoop *join_node = (NestLoop *) NULL;

    if (IsA(inner_node, IndexScan)) {
        /*  An index is being used to reduce the number of tuples scanned in 
         *    the inner relation.
         * There will never be more than one index used in the inner 
         * scan path, so we need only consider the first set of 
         *    qualifications in indxqual. 
         */

        List *inner_indxqual = lfirst(((IndexScan *) inner_node)->indxqual);
        List *inner_qual = (inner_indxqual == NULL) ? NULL : lfirst(inner_indxqual);

        /* If we have in fact found a join index qualification, remove these
         * index clauses from the nestloop's join clauses and reset the 
         * inner(index) scan's qualification so that the var nodes refer to
         * the proper outer join relation attributes.
         */
        if (!(qual_clause_p((Node *) inner_qual))) {
            List *new_inner_qual = NIL;

            clauses = set_difference(clauses, inner_indxqual);
            new_inner_qual =
                    index_outerjoin_references(inner_indxqual,
                                               outer_node->targetlist,
                                               ((Scan *) inner_node)->scanrelid);
            ((IndexScan *) inner_node)->indxqual =
                    lcons(new_inner_qual, NIL);
        }
    } else if (IsA_Join(inner_node)) {
        inner_node = (Plan *) make_temp(inner_tlist,
                                        NIL,
                                        NULL,
                                        inner_node,
                                        TEMP_MATERIAL);
    }

    join_node = make_nestloop(tlist,
                              join_references(clauses,
                                              outer_tlist,
                                              inner_tlist),
                              outer_node,
                              inner_node);

    join_node->join.cost = best_path->path.path_cost;

    return (join_node);
}

static MergeJoin *
create_mergejoin_node(MergePath *best_path,
                      List *tlist,
                      List *clauses,
                      Plan *outer_node,
                      List *outer_tlist,
                      Plan *inner_node,
                      List *inner_tlist) {
    List *qpqual, *mergeclauses;
    RegProcedure opcode;
    Oid *outer_order, *inner_order;
    MergeJoin *join_node;


    /* Separate the mergeclauses from the other join qualification 
     * clauses and set those clauses to contain references to lower 
     * attributes. 
     */
    qpqual = join_references(set_difference(clauses,
                                            best_path->path_mergeclauses),
                             outer_tlist,
                             inner_tlist);

    /* Now set the references in the mergeclauses and rearrange them so 
     * that the outer variable is always on the left. 
     */
    mergeclauses = switch_outer(join_references(best_path->path_mergeclauses,
                                                outer_tlist,
                                                inner_tlist));

    opcode =
            get_opcode((best_path->jpath.path.p_ordering.ord.merge)->join_operator);

    outer_order = (Oid *) palloc(sizeof(Oid) * 2);
    outer_order[0] =
            (best_path->jpath.path.p_ordering.ord.merge)->left_operator;
    outer_order[1] = 0;

    inner_order = (Oid *) palloc(sizeof(Oid) * 2);
    inner_order[0] =
            (best_path->jpath.path.p_ordering.ord.merge)->right_operator;
    inner_order[1] = 0;

    /* Create explicit sort paths for the outer and inner join paths if 
     * necessary.  The sort cost was already accounted for in the path. 
     */
    if (best_path->outersortkeys) {
        Temp *sorted_outer_node = make_temp(outer_tlist,
                                            best_path->outersortkeys,
                                            outer_order,
                                            outer_node,
                                            TEMP_SORT);
        sorted_outer_node->plan.cost = outer_node->cost;
        outer_node = (Plan *) sorted_outer_node;
    }

    if (best_path->innersortkeys) {
        Temp *sorted_inner_node = make_temp(inner_tlist,
                                            best_path->innersortkeys,
                                            inner_order,
                                            inner_node,
                                            TEMP_SORT);
        sorted_inner_node->plan.cost = outer_node->cost;
        inner_node = (Plan *) sorted_inner_node;
    }

    join_node = make_mergesort(tlist,
                               qpqual,
                               mergeclauses,
                               opcode,
                               inner_order,
                               outer_order,
                               inner_node,
                               outer_node);

    join_node->join.cost = best_path->jpath.path.path_cost;

    return (join_node);
}

/*    
 * create_hashjoin_node--			XXX HASH
 *    
 *    Returns a new hashjoin node.
 *    
 *    XXX hash join ops are totally bogus -- how the hell do we choose
 *    	these??  at runtime?  what about a hash index?
 */
static HashJoin *
create_hashjoin_node(HashPath *best_path,
                     List *tlist,
                     List *clauses,
                     Plan *outer_node,
                     List *outer_tlist,
                     Plan *inner_node,
                     List *inner_tlist) {
    List *qpqual;
    List *hashclauses;
    HashJoin *join_node;
    Hash *hash_node;
    Var *innerhashkey;

    /* Separate the hashclauses from the other join qualification clauses
     * and set those clauses to contain references to lower attributes. 
     */
    qpqual =
            join_references(set_difference(clauses,
                                           best_path->path_hashclauses),
                            outer_tlist,
                            inner_tlist);

    /* Now set the references in the hashclauses and rearrange them so 
     * that the outer variable is always on the left. 
     */
    hashclauses =
            switch_outer(join_references(best_path->path_hashclauses,
                                         outer_tlist,
                                         inner_tlist));

    innerhashkey = get_rightop(lfirst(hashclauses));

    hash_node = make_hash(inner_tlist, innerhashkey, inner_node);
    join_node = make_hashjoin(tlist,
                              qpqual,
                              hashclauses,
                              outer_node,
                              (Plan *) hash_node);
    join_node->join.cost = best_path->jpath.path.path_cost;

    return (join_node);
}


/*****************************************************************************
 *
 *  SUPPORTING ROUTINES
 *
 *****************************************************************************/

static Node *
fix_indxqual_references(Node *clause, Path *index_path) {
    Node *newclause;

    if (IsA(clause, Var)) {
        if (lfirsti(index_path->parent->relids) == ((Var *) clause)->varno) {
            int pos = 0;
            int varatt = ((Var *) clause)->varattno;
            int *indexkeys = index_path->parent->indexkeys;

            if (indexkeys) {
                while (indexkeys[pos] != 0) {
                    if (varatt == indexkeys[pos]) {
                        break;
                    }
                    pos++;
                }
            }
            newclause = copyObject((Node *) clause);
            ((Var *) newclause)->varattno = pos + 1;
            return (newclause);
        } else {
            return (clause);
        }
    } else if (IsA(clause, Const)) {
        return (clause);
    } else if (is_opclause(clause) &&
               is_funcclause((Node *) get_leftop((Expr *) clause)) &&
               ((Func *) ((Expr *) get_leftop((Expr *) clause))->oper)->funcisindex) {
        Var *newvar =
                makeVar((Index) lfirst(index_path->parent->relids),
                        1, /* func indices have one key */
                        ((Func *) ((Expr *) clause)->oper)->functype,
                        (Index) lfirst(index_path->parent->relids),
                        0);

        return
                ((Node *) make_opclause((Oper *) ((Expr *) clause)->oper,
                                        newvar,
                                        get_rightop((Expr *) clause)));

    } else if (IsA(clause, Expr)) {
        Expr *expr = (Expr *) clause;
        List *new_subclauses = NIL;
        Node *subclause = NULL;
        List *i = NIL;

        foreach(i, expr->args) {
            subclause = lfirst(i);
            if (subclause)
                new_subclauses =
                        lappend(new_subclauses,
                                fix_indxqual_references(subclause,
                                                        index_path));

        }

        /* XXX new_subclauses should be a list of the form:
         * ( (var var) (var const) ...) ?
         */
        if (new_subclauses) {
            return (Node *)
                    make_clause(expr->opType, expr->oper, new_subclauses);
        } else {
            return (clause);
        }
    } else {
        List *oldclauses = (List *) clause;
        List *new_subclauses = NIL;
        Node *subclause = NULL;
        List *i = NIL;

        foreach(i, oldclauses) {
            subclause = lfirst(i);
            if (subclause)
                new_subclauses =
                        lappend(new_subclauses,
                                fix_indxqual_references(subclause,
                                                        index_path));

        }

        /* XXX new_subclauses should be a list of the form:
         * ( (var var) (var const) ...) ?
         */
        if (new_subclauses) {
            return (Node *) new_subclauses;
        } else {
            return (clause);
        }
    }
}


/*    
 * switch_outer--
 *    Given a list of merge clauses, rearranges the elements within the
 *    clauses so the outer join variable is on the left and the inner is on
 *    the right.
 *    
 *    Returns the rearranged list ?
 *    
 *    XXX Shouldn't the operator be commuted?!
 */
static List *
switch_outer(List *clauses) {
    List *t_list = NIL;
    Expr *temp = NULL;
    List *i = NIL;
    Expr *clause;

    foreach(i, clauses) {
        clause = lfirst(i);
        if (var_is_outer(get_rightop(clause))) {
            temp = make_clause(clause->opType, clause->oper,
                               lcons(get_rightop(clause),
                                     lcons(get_leftop(clause),
                                           NIL)));
            t_list = lappend(t_list, temp);
        } else
            t_list = lappend(t_list, clause);
    }
    return (t_list);
}

/*    
 * set-temp-tlist-operators--
 *    Sets the key and keyop fields of resdom nodes in a target list.
 *    
 *    'tlist' is the target list
 *    'pathkeys' is a list of N keys in the form((key1) (key2)...(keyn)),
 *    		corresponding to vars in the target list that are to
 *    		be sorted or hashed
 *    'operators' is the corresponding list of N sort or hash operators
 *    'keyno' is the first key number 
 *    XXX - keyno ? doesn't exist - jeff
 *    
 *    Returns the modified target list.
 */
static List *
set_temp_tlist_operators(List *tlist, List *pathkeys, Oid *operators) {
    Node *keys = NULL;
    int keyno = 1;
    Resdom *resdom = (Resdom *) NULL;
    List *i = NIL;

    foreach(i, pathkeys) {
        keys = lfirst((List *) lfirst(i));
        resdom = tlist_member((Var *) keys, tlist);
        if (resdom) {

            /* Order the resdom keys and replace the operator OID for each 
             *    key with the regproc OID. 
             *
             * XXX Note that the optimizer only generates merge joins 
             *    with 1 operator (see create_mergejoin_node)  - ay 2/95
             */
            resdom->reskey = keyno;
            resdom->reskeyop = get_opcode(operators[0]);
        }
        keyno += 1;
    }
    return (tlist);
}

/*****************************************************************************
 *
 *
 *****************************************************************************/

/*    
 * make_temp--
 *    Create plan nodes to sort or materialize relations into temporaries. The
 *    result returned for a sort will look like (SEQSCAN(SORT(plan-node)))
 *    or (SEQSCAN(MATERIAL(plan-node)))
 *    
 *    'tlist' is the target list of the scan to be sorted or hashed
 *    'keys' is the list of keys which the sort or hash will be done on
 *    'operators' is the operators with which the sort or hash is to be done
 *    	(a list of operator OIDs)
 *    'plan-node' is the node which yields tuples for the sort
 *    'temptype' indicates which operation(sort or hash) to perform
 */
static Temp *
make_temp(List *tlist,
          List *keys,
          Oid *operators,
          Plan *plan_node,
          int temptype) {
    List *temp_tlist;
    Temp *retval;

    /*    Create a new target list for the temporary, with keys set. */
    temp_tlist = set_temp_tlist_operators(new_unsorted_tlist(tlist),
                                          keys,
                                          operators);
    switch (temptype) {
        case TEMP_SORT :
            retval = (Temp *) make_seqscan(tlist,
                                           NIL,
                                           _TEMP_RELATION_ID_,
                                           (Plan *) make_sort(temp_tlist,
                                                              _TEMP_RELATION_ID_,
                                                              plan_node,
                                                              length(keys)));
            break;

        case TEMP_MATERIAL :
            retval = (Temp *) make_seqscan(tlist,
                                           NIL,
                                           _TEMP_RELATION_ID_,
                                           (Plan *) make_material(temp_tlist,
                                                                  _TEMP_RELATION_ID_,
                                                                  plan_node,
                                                                  length(keys)));
            break;

        default:
            elog(WARN, "make_temp: unknown temp type %d", temptype);

    }
    return (retval);
}


SeqScan *
make_seqscan(List *qptlist,
             List *qpqual,
             Index scanrelid,
             Plan *lefttree) {
    SeqScan *node = makeNode(SeqScan);
    Plan *plan = &node->plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = qptlist;
    plan->qual = qpqual;
    plan->lefttree = lefttree;
    plan->righttree = NULL;
    node->scanrelid = scanrelid;
    node->scanstate = (CommonScanState *) NULL;

    return (node);
}

static IndexScan *
make_indexscan(List *qptlist,
               List *qpqual,
               Index scanrelid,
               List *indxid,
               List *indxqual) {
    IndexScan *node = makeNode(IndexScan);
    Plan *plan = &node->scan.plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = qptlist;
    plan->qual = qpqual;
    plan->lefttree = NULL;
    plan->righttree = NULL;
    node->scan.scanrelid = scanrelid;
    node->indxid = indxid;
    node->indxqual = indxqual;
    node->scan.scanstate = (CommonScanState *) NULL;

    return (node);
}


static NestLoop *
make_nestloop(List *qptlist,
              List *qpqual,
              Plan *lefttree,
              Plan *righttree) {
    NestLoop *node = makeNode(NestLoop);
    Plan *plan = &node->join;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = qptlist;
    plan->qual = qpqual;
    plan->lefttree = lefttree;
    plan->righttree = righttree;
    node->nlstate = (NestLoopState *) NULL;

    return (node);
}

static HashJoin *
make_hashjoin(List *tlist,
              List *qpqual,
              List *hashclauses,
              Plan *lefttree,
              Plan *righttree) {
    HashJoin *node = makeNode(HashJoin);
    Plan *plan = &node->join;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = qpqual;
    plan->lefttree = lefttree;
    plan->righttree = righttree;
    node->hashclauses = hashclauses;
    node->hashjointable = NULL;
    node->hashjointablekey = 0;
    node->hashjointablesize = 0;
    node->hashdone = false;

    return (node);
}

static Hash *
make_hash(List *tlist, Var *hashkey, Plan *lefttree) {
    Hash *node = makeNode(Hash);
    Plan *plan = &node->plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = NULL;
    plan->lefttree = lefttree;
    plan->righttree = NULL;
    node->hashkey = hashkey;
    node->hashtable = NULL;
    node->hashtablekey = 0;
    node->hashtablesize = 0;

    return (node);
}

static MergeJoin *
make_mergesort(List *tlist,
               List *qpqual,
               List *mergeclauses,
               Oid opcode,
               Oid *rightorder,
               Oid *leftorder,
               Plan *righttree,
               Plan *lefttree) {
    MergeJoin *node = makeNode(MergeJoin);
    Plan *plan = &node->join;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = qpqual;
    plan->lefttree = lefttree;
    plan->righttree = righttree;
    node->mergeclauses = mergeclauses;
    node->mergesortop = opcode;
    node->mergerightorder = rightorder;
    node->mergeleftorder = leftorder;

    return (node);
}

Sort *
make_sort(List *tlist, Oid tempid, Plan *lefttree, int keycount) {
    Sort *node = makeNode(Sort);
    Plan *plan = &node->plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = NIL;
    plan->lefttree = lefttree;
    plan->righttree = NULL;
    node->tempid = tempid;
    node->keycount = keycount;

    return (node);
}

static Material *
make_material(List *tlist,
              Oid tempid,
              Plan *lefttree,
              int keycount) {
    Material *node = makeNode(Material);
    Plan *plan = &node->plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = NIL;
    plan->lefttree = lefttree;
    plan->righttree = NULL;
    node->tempid = tempid;
    node->keycount = keycount;

    return (node);
}

Agg *
make_agg(List *tlist, int nagg, Aggreg **aggs) {
    Agg *node = makeNode(Agg);

    node->plan.cost = 0.0;
    node->plan.state = (EState *) NULL;
    node->plan.qual = NULL;
    node->plan.targetlist = tlist;
    node->plan.lefttree = (Plan *) NULL;
    node->plan.righttree = (Plan *) NULL;
    node->numAgg = nagg;
    node->aggs = aggs;

    return (node);
}

Group *
make_group(List *tlist,
           bool tuplePerGroup,
           int ngrp,
           AttrNumber *grpColIdx,
           Sort *lefttree) {
    Group *node = makeNode(Group);

    node->plan.cost = 0.0;
    node->plan.state = (EState *) NULL;
    node->plan.qual = NULL;
    node->plan.targetlist = tlist;
    node->plan.lefttree = (Plan *) lefttree;
    node->plan.righttree = (Plan *) NULL;
    node->tuplePerGroup = tuplePerGroup;
    node->numCols = ngrp;
    node->grpColIdx = grpColIdx;

    return (node);
}

/*
 *  A unique node always has a SORT node in the lefttree.
 *
 *  the uniqueAttr argument must be a null-terminated string,
 * either the name of the attribute to select unique on 
 * or "*"
 */

Unique *
make_unique(List *tlist, Plan *lefttree, char *uniqueAttr) {
    Unique *node = makeNode(Unique);
    Plan *plan = &node->plan;

    plan->cost = 0.0;
    plan->state = (EState *) NULL;
    plan->targetlist = tlist;
    plan->qual = NIL;
    plan->lefttree = lefttree;
    plan->righttree = NULL;
    node->tempid = _TEMP_RELATION_ID_;
    node->keycount = 0;
    if (strcmp(uniqueAttr, "*") == 0)
        node->uniqueAttr = NULL;
    else {
        node->uniqueAttr = pstrdup(uniqueAttr);
    }
    return (node);
}

List *generate_fjoin(List *tlist) {
#if 0
    List tlistP;
    List newTlist = NIL;
    List fjoinList = NIL;
    int  nIters = 0;

    /*
     * Break the target list into elements with Iter nodes,
     * and those without them.
     */
    foreach(tlistP, tlist) {
    List tlistElem;

    tlistElem = lfirst(tlistP);
    if (IsA(lsecond(tlistElem),Iter)) {
        nIters++;
        fjoinList = lappend(fjoinList, tlistElem);
    } else {
        newTlist = lappend(newTlist, tlistElem);
    }
    }

    /*
     * if we have an Iter node then we need to flatten.
     */
    if (nIters > 0) {
    List *inner;
    List      *tempList;
    Fjoin     *fjoinNode;
    DatumPtr  results = (DatumPtr)palloc(nIters*sizeof(Datum));
    BoolPtr   alwaysDone = (BoolPtr)palloc(nIters*sizeof(bool));

    inner = lfirst(fjoinList);
    fjoinList = lnext(fjoinList);
    fjoinNode = (Fjoin)MakeFjoin(false,
                     nIters,
                     inner,
                     results,
                     alwaysDone);
    tempList = lcons(fjoinNode, NIL);
    tempList = nconc(tempList, fjoinList);
    newTlist = lappend(newTlist, tempList);
    }
    return newTlist;
#endif
    return tlist;    /* do nothing for now - ay 10/94 */
}
