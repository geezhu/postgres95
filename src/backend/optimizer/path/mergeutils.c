/*-------------------------------------------------------------------------
 *
 * mergeutils.c--
 *    Utilities for finding applicable merge clauses and pathkeys
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/optimizer/path/Attic/mergeutils.c,v 1.1.1.1 1996/07/09 06:21:36 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "nodes/pg_list.h"
#include "nodes/relation.h"

#include "optimizer/internal.h"
#include "optimizer/paths.h"
#include "optimizer/clauses.h"
#include "optimizer/ordering.h"

/*    
 * group-clauses-by-order--
 *    If a join clause node in 'clauseinfo-list' is mergesortable, store
 *    it within a mergeinfo node containing other clause nodes with the same
 *    mergesort ordering.
 *    
 * 'clauseinfo-list' is the list of clauseinfo nodes
 * 'inner-relid' is the relid of the inner join relation
 *    
 * Returns the new list of mergeinfo nodes.
 *    
 */
List *
group_clauses_by_order(List *clauseinfo_list,
                       int inner_relid) {
    List *mergeinfo_list = NIL;
    List *xclauseinfo = NIL;

    foreach (xclauseinfo, clauseinfo_list) {
        CInfo *clauseinfo = (CInfo *) lfirst(xclauseinfo);
        MergeOrder *merge_ordering = clauseinfo->mergesortorder;

        if (merge_ordering) {
            /*
             * Create a new mergeinfo node and add it to
             * 'mergeinfo-list' if one does not yet exist for this
             * merge ordering.
             */
            PathOrder p_ordering;
            MInfo *xmergeinfo;
            Expr *clause = clauseinfo->clause;
            Var *leftop = get_leftop(clause);
            Var *rightop = get_rightop(clause);
            JoinKey *keys;

            p_ordering.ordtype = MERGE_ORDER;
            p_ordering.ord.merge = merge_ordering;
            xmergeinfo =
                    match_order_mergeinfo(&p_ordering, mergeinfo_list);
            if (inner_relid == leftop->varno) {
                keys = makeNode(JoinKey);
                keys->outer = rightop;
                keys->inner = leftop;
            } else {
                keys = makeNode(JoinKey);
                keys->outer = leftop;
                keys->inner = rightop;
            }

            if (xmergeinfo == NULL) {
                xmergeinfo = makeNode(MInfo);

                xmergeinfo->m_ordering = merge_ordering;
                mergeinfo_list = lcons(xmergeinfo,
                                       mergeinfo_list);
            }

            ((JoinMethod *) xmergeinfo)->clauses =
                    lcons(clause,
                          ((JoinMethod *) xmergeinfo)->clauses);
            ((JoinMethod *) xmergeinfo)->jmkeys =
                    lcons(keys,
                          ((JoinMethod *) xmergeinfo)->jmkeys);
        }
    }
    return (mergeinfo_list);
}


/*    
 * match-order-mergeinfo--
 *    Searches the list 'mergeinfo-list' for a mergeinfo node whose order
 *    field equals 'ordering'.
 *    
 * Returns the node if it exists.
 *    
 */
MInfo *
match_order_mergeinfo(PathOrder *ordering, List *mergeinfo_list) {
    MergeOrder *xmergeorder;
    List *xmergeinfo = NIL;

    foreach(xmergeinfo, mergeinfo_list) {
        MInfo *mergeinfo = (MInfo *) lfirst(xmergeinfo);

        xmergeorder = mergeinfo->m_ordering;

        if ((ordering->ordtype == MERGE_ORDER &&
             equal_merge_merge_ordering(ordering->ord.merge, xmergeorder)) ||
            (ordering->ordtype == SORTOP_ORDER &&
             equal_path_merge_ordering(ordering->ord.sortop, xmergeorder))) {

            return (mergeinfo);
        }
    }
    return ((MInfo *) NIL);
}
