/*-------------------------------------------------------------------------
 *
 * prune.c--
 *    Routines to prune redundant paths and relations
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/optimizer/path/Attic/prune.c,v 1.1.1.1 1996/07/09 06:21:36 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "nodes/pg_list.h"
#include "nodes/relation.h"

#include "optimizer/internal.h"
#include "optimizer/cost.h"
#include "optimizer/paths.h"
#include "optimizer/pathnode.h"

#include "utils/elog.h"


static List *prune_joinrel(Rel *rel, List *other_rels);

/*    
 * prune-joinrels--
 *    Removes any redundant relation entries from a list of rel nodes
 *    'rel-list'.
 *    
 * Returns the resulting list. 
 *    
 */
List *prune_joinrels(List *rel_list) {
    List *temp_list = NIL;

    if (rel_list != NIL) {
        temp_list = lcons(lfirst(rel_list),
                          prune_joinrels(prune_joinrel((Rel *) lfirst(rel_list),
                                                       lnext(rel_list))));
    }
    return (temp_list);
}

/*    
 * prune-joinrel--
 *    Prunes those relations from 'other-rels' that are redundant with
 *    'rel'.  A relation is redundant if it is built up of the same
 *    relations as 'rel'.  Paths for the redundant relation are merged into
 *    the pathlist of 'rel'.
 *    
 * Returns a list of non-redundant relations, and sets the pathlist field
 * of 'rel' appropriately.
 *    
 */
static List *
prune_joinrel(Rel *rel, List *other_rels) {
    List *i = NIL;
    List *t_list = NIL;
    List *temp_node = NIL;
    Rel *other_rel = (Rel *) NULL;

    foreach(i, other_rels) {
        other_rel = (Rel *) lfirst(i);
        if (same(rel->relids, other_rel->relids)) {
            rel->pathlist = add_pathlist(rel,
                                         rel->pathlist,
                                         other_rel->pathlist);
            t_list = nconc(t_list, NIL);  /* XXX is this right ? */
        } else {
            temp_node = lcons(other_rel, NIL);
            t_list = nconc(t_list, temp_node);
        }
    }
    return (t_list);
}

/*    
 * prune-rel-paths--
 *    For each relation entry in 'rel-list' (which corresponds to a join
 *    relation), set pointers to the unordered path and cheapest paths
 *    (if the unordered path isn't the cheapest, it is pruned), and
 *    reset the relation's size field to reflect the join.
 *    
 * Returns nothing of interest.
 *    
 */
void
prune_rel_paths(List *rel_list) {
    List *x = NIL;
    List *y = NIL;
    Path *path;
    Rel *rel = (Rel *) NULL;
    JoinPath *cheapest = (JoinPath *) NULL;

    foreach(x, rel_list) {
        rel = (Rel *) lfirst(x);
        foreach(y, rel->pathlist) {
            path = (Path *) lfirst(y);

            if (!path->p_ordering.ord.sortop) {
                break;
            }
        }
        cheapest = (JoinPath *) prune_rel_path(rel, path);
        if (IsA_JoinPath(cheapest)) {
            rel->size = compute_joinrel_size(cheapest);
        } else
            elog(WARN, "non JoinPath called");
    }
}


/*    
 * prune-rel-path--
 *    Compares the unordered path for a relation with the cheapest path. If
 *    the unordered path is not cheapest, it is pruned.
 *    
 *    Resets the pointers in 'rel' for unordered and cheapest paths.
 *    
 * Returns the cheapest path.
 *    
 */
Path *
prune_rel_path(Rel *rel, Path *unorderedpath) {
    Path *cheapest = set_cheapest(rel, rel->pathlist);

    /* don't prune if not pruneable  -- JMH, 11/23/92 */
    if (unorderedpath != cheapest
        && rel->pruneable) {

        rel->unorderedpath = (Path *) NULL;
        rel->pathlist = lremove(unorderedpath, rel->pathlist);
    } else {
        rel->unorderedpath = (Path *) unorderedpath;
    }

    return (cheapest);
}

/*
 * merge-joinrels--
 *    Given two lists of rel nodes that are already
 *    pruned, merge them into one pruned rel node list
 *
 * 'rel-list1' and
 * 'rel-list2' are the rel node lists
 *
 * Returns one pruned rel node list
 */
List *
merge_joinrels(List *rel_list1, List *rel_list2) {
    List *xrel = NIL;

    foreach(xrel, rel_list1) {
        Rel *rel = (Rel *) lfirst(xrel);
        rel_list2 = prune_joinrel(rel, rel_list2);
    }
    return (append(rel_list1, rel_list2));
}

/*
 * prune_oldrels--
 *    If all the joininfo's in a rel node are inactive,
 *    that means that this node has been joined into
 *    other nodes in all possible ways, therefore
 *    this node can be discarded.  If not, it will cause
 *    extra complexity of the optimizer.
 *
 * old_rels is a list of rel nodes
 *	
 * Returns a new list of rel nodes
 */
List *prune_oldrels(List *old_rels) {
    Rel *rel;
    List *joininfo_list, *xjoininfo;

    if (old_rels == NIL)
        return (NIL);

    rel = (Rel *) lfirst(old_rels);
    joininfo_list = rel->joininfo;
    if (joininfo_list == NIL)
        return (lcons(rel, prune_oldrels(lnext(old_rels))));

    foreach(xjoininfo, joininfo_list) {
        JInfo *joininfo = (JInfo *) lfirst(xjoininfo);
        if (!joininfo->inactive)
            return (lcons(rel, prune_oldrels(lnext(old_rels))));
    }
    return (prune_oldrels(lnext(old_rels)));
}
