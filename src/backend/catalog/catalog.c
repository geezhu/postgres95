/*-------------------------------------------------------------------------
 *
 * catalog.c--
 *    
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/catalog/catalog.c,v 1.1.1.1 1996/07/09 06:21:15 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <string.h>    /* XXX */
#include "postgres.h"
#include "miscadmin.h"  /* for DataDir */
#include "access/htup.h"
#include "storage/buf.h"
#include "utils/elog.h"
#include "utils/palloc.h"

#include "utils/syscache.h"
#include "catalog/catname.h"    /* NameIs{,Shared}SystemRelationName */
#include "catalog/pg_attribute.h"
#include "catalog/pg_type.h"
#include "catalog/catalog.h"
#include "storage/bufmgr.h"
#include "access/transam.h"


#ifndef    MAXPATHLEN
#define    MAXPATHLEN    80
#endif

/*
 * relpath		- path to the relation
 *	Perhaps this should be in-line code in relopen().
 */
char *
relpath(char relname[]) {
    char *path;

    if (IsSharedSystemRelationName(relname)) {
        path = (char *) palloc(strlen(DataDir) + sizeof(NameData) + 2);
        sprintf(path, "%s/%.*s", DataDir, NAMEDATALEN, relname);
        return (path);
    }
    return (relname);
}

/*
 * issystem	- returns non-zero iff relname is a system catalog
 *
 *	We now make a new requirement where system catalog relns must begin
 *	with pg_ while user relns are forbidden to do so.  Make the test
 * 	trivial and instantaneous.
 *
 *	XXX this is way bogus. -- pma
 */
bool
issystem(char relname[]) {
    if (relname[0] && relname[1] && relname[2])
        return (relname[0] == 'p' &&
                relname[1] == 'g' &&
                relname[2] == '_');
    else
        return FALSE;
}

/*
 * IsSystemRelationName --
 *	True iff name is the name of a system catalog relation.
 *
 *	We now make a new requirement where system catalog relns must begin
 *	with pg_ while user relns are forbidden to do so.  Make the test
 * 	trivial and instantaneous.
 *
 *	XXX this is way bogus. -- pma
 */
bool
IsSystemRelationName(char *relname) {
    if (relname[0] && relname[1] && relname[2])
        return (relname[0] == 'p' &&
                relname[1] == 'g' &&
                relname[2] == '_');
    else
        return FALSE;
}

/*
 * IsSharedSystemRelationName --
 *	True iff name is the name of a shared system catalog relation.
 */
bool
IsSharedSystemRelationName(char *relname) {
    int i;

    /*
     * Quick out: if it's not a system relation, it can't be a shared
     * system relation.
     */
    if (!IsSystemRelationName(relname))
        return FALSE;

    i = 0;
    while (SharedSystemRelationNames[i] != NULL) {
        if (strcmp(SharedSystemRelationNames[i], relname) == 0)
            return TRUE;
        i++;
    }
    return FALSE;
}

/*
 *	newoid		- returns a unique identifier across all catalogs.
 *
 *	Object Id allocation is now done by GetNewObjectID in
 *	access/transam/varsup.c.  oids are now allocated correctly.
 *
 * old comments:
 *	This needs to change soon, it fails if there are too many more
 *	than one call per second when postgres restarts after it dies.
 *
 *	The distribution of OID's should be done by the POSTMASTER.
 *	Also there needs to be a facility to preallocate OID's.  Ie.,
 *	for a block of OID's to be declared as invalid ones to allow
 *	user programs to use them for temporary object identifiers.
 */
Oid newoid() {
    Oid lastoid;

    GetNewObjectId(&lastoid);
    if (!OidIsValid(lastoid))
        elog(WARN, "newoid: GetNewObjectId returns invalid oid");
    return lastoid;
}

/*
 *	fillatt		- fills the ATTRIBUTE relation fields from the TYP
 *
 *	Expects that the atttypid domain is set for each att[].
 *	Returns with the attnum, and attlen domains set.
 *	attnum, attproc, atttyparg, ... should be set by the user.
 *
 *	In the future, attnum may not be set?!? or may be passed as an arg?!?
 *
 *	Current implementation is very inefficient--should cashe the
 *	information if this is at all possible.
 *
 *	Check to see if this is really needed, and especially in the case
 *	of index tuples.
 */
void
fillatt(TupleDesc tupleDesc) {
    AttributeTupleForm *attributeP;
    register TypeTupleForm typp;
    HeapTuple tuple;
    int i;
    int natts = tupleDesc->natts;
    AttributeTupleForm *att = tupleDesc->attrs;

    if (natts < 0 || natts > MaxHeapAttributeNumber)
        elog(WARN, "fillatt: %d attributes is too large", natts);
    if (natts == 0) {
        elog(DEBUG, "fillatt: called with natts == 0");
        return;
    }

    attributeP = &att[0];

    for (i = 0; i < natts;) {
        tuple = SearchSysCacheTuple(TYPOID,
                                    Int32GetDatum((*attributeP)->atttypid),
                                    0, 0, 0);
        if (!HeapTupleIsValid(tuple)) {
            elog(WARN, "fillatt: unknown atttypid %ld",
                 (*attributeP)->atttypid);
        } else {
            (*attributeP)->attnum = (int16) ++i;
            /* Check if the attr is a set before messing with the length
               and byval, since those were already set in 
               TupleDescInitEntry.  In fact, this seems redundant 
               here, but who knows what I'll break if I take it out...
    
               same for char() and varchar() stuff. I share the same
               sentiments. This function is poorly written anyway. -ay 6/95
               */
            if (!(*attributeP)->attisset &&
                (*attributeP)->atttypid != BPCHAROID &&
                (*attributeP)->atttypid != VARCHAROID) {

                typp = (TypeTupleForm) GETSTRUCT(tuple);  /* XXX */
                (*attributeP)->attlen = typp->typlen;
                (*attributeP)->attbyval = typp->typbyval;
            }
        }
        attributeP += 1;
    }
}
