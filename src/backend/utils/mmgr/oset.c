/*-------------------------------------------------------------------------
 *
 * oset.c--
 *    Fixed format ordered set definitions.
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/utils/mmgr/Attic/oset.c,v 1.1.1.1 1996/07/09 06:22:09 scrappy Exp $
 *
 * NOTE
 *    XXX This is a preliminary implementation which lacks fail-fast
 *    XXX validity checking of arguments.
 *
 *-------------------------------------------------------------------------
 */
#include "c.h"

#include "utils/memutils.h"    /* where declarations of this file goes */

static Pointer OrderedElemGetBase(OrderedElem elem);

static void OrderedElemInit(OrderedElem elem, OrderedSet set);

static void OrderedElemPush(OrderedElem elem);

static void OrderedElemPushHead(OrderedElem elem);

/*
 * OrderedElemGetBase --
 *	Returns base of enclosing structure.
 */
static Pointer
OrderedElemGetBase(OrderedElem elem) {
    if (elem == (OrderedElem) NULL)
        return (Pointer) NULL;

    return ((Pointer) ((char *) (elem) - (elem)->set->offset));
}

/*
 * OrderedSetInit --
 */
void
OrderedSetInit(OrderedSet set, Offset offset) {
    set->head = (OrderedElem) &set->dummy;
    set->dummy = NULL;
    set->tail = (OrderedElem) &set->head;
    set->offset = offset;
}

/*
 * OrderedElemInit --
 */
static void
OrderedElemInit(OrderedElem elem, OrderedSet set) {
    elem->set = set;
    /* mark as unattached */
    elem->next = NULL;
    elem->prev = NULL;
}

/*
 * OrderedSetContains --
 *	True iff ordered set contains given element.
 */
bool
OrderedSetContains(OrderedSet set, OrderedElem elem) {
    return ((bool) (elem->set == set && (elem->next || elem->prev)));
}

/*
 * OrderedSetGetHead --
 */
Pointer
OrderedSetGetHead(OrderedSet set) {
    register OrderedElem elem;

    elem = set->head;
    if (elem->next) {
        return (OrderedElemGetBase(elem));
    }
    return (NULL);
}

/*
 * OrderedSetGetTail --
 */
Pointer
OrderedSetGetTail(OrderedSet set) {
    register OrderedElem elem;

    elem = set->tail;
    if (elem->prev) {
        return (OrderedElemGetBase(elem));
    }
    return (NULL);
}

/*
 * OrderedElemGetPredecessor --
 */
Pointer
OrderedElemGetPredecessor(OrderedElem elem) {
    elem = elem->prev;
    if (elem->prev) {
        return (OrderedElemGetBase(elem));
    }
    return (NULL);
}

/*
 * OrderedElemGetSuccessor --
 */
Pointer
OrderedElemGetSuccessor(OrderedElem elem) {
    elem = elem->next;
    if (elem->next) {
        return (OrderedElemGetBase(elem));
    }
    return (NULL);
}

/*
 * OrderedElemPop --
 */
void
OrderedElemPop(OrderedElem elem) {
    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;
    /* assignments used only for error detection */
    elem->next = NULL;
    elem->prev = NULL;
}

/*
 * OrderedElemPushInto --
 */
void
OrderedElemPushInto(OrderedElem elem, OrderedSet set) {
    OrderedElemInit(elem, set);
    OrderedElemPush(elem);
}

/*
 * OrderedElemPush --
 */
static void
OrderedElemPush(OrderedElem elem) {
    OrderedElemPushHead(elem);
}

/*
 * OrderedElemPushHead --
 */
static void
OrderedElemPushHead(OrderedElem elem) {
    elem->next = elem->set->head;
    elem->prev = (OrderedElem) &elem->set->head;
    elem->next->prev = elem;
    elem->prev->next = elem;
}

