/*-------------------------------------------------------------------------
 *
 * pg_type.c--
 *    routines to support manipulation of the pg_type relation
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/catalog/pg_type.c,v 1.1.1.1 1996/07/09 06:21:18 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <string.h>
#include "postgres.h"

#include "access/heapam.h"
#include "access/relscan.h"
#include "access/skey.h"
#include "access/tupdesc.h"
#include "utils/builtins.h"
#include "utils/rel.h"
#include "utils/palloc.h"
#include "fmgr.h"
#include "utils/elog.h"
#include "parser/catalog_utils.h"

#include "catalog/catname.h"
#include "utils/syscache.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "catalog/indexing.h"
#include "storage/lmgr.h"

/* ----------------------------------------------------------------
 * 	TypeGetWithOpenRelation
 *
 *	preforms a scan on pg_type for a type tuple with the
 *	given type name.
 * ----------------------------------------------------------------
 *	pg_type_desc		 -- reldesc for pg_type
 *	typeName		 -- name of type to be fetched 
 *	defined			 -- has the type been defined? 
 */
static Oid
TypeGetWithOpenRelation(Relation pg_type_desc,
                        char *typeName,
                        bool *defined) {
    HeapScanDesc scan;
    HeapTuple tup;

    static ScanKeyData typeKey[1] = {
            {0, Anum_pg_type_typname, NameEqualRegProcedure}
    };

    /* ----------------
     *	initialize the scan key and begin a scan of pg_type
     * ----------------
     */
    fmgr_info(NameEqualRegProcedure,
              &typeKey[0].sk_func, &typeKey[0].sk_nargs);
    typeKey[0].sk_argument = PointerGetDatum(typeName);

    scan = heap_beginscan(pg_type_desc,
                          0,
                          SelfTimeQual,
                          1,
                          typeKey);

    /* ----------------
     *	get the type tuple, if it exists.
     * ----------------
     */
    tup = heap_getnext(scan, 0, (Buffer *) 0);

    /* ----------------
     *	if no type tuple exists for the given type name, then
     *  end the scan and return appropriate information.
     * ----------------
     */
    if (!HeapTupleIsValid(tup)) {
        heap_endscan(scan);
        *defined = false;
        return InvalidOid;
    }

    /* ----------------
     *	here, the type tuple does exist so we pull information from
     *  the typisdefined field of the tuple and return the tuple's
     *  oid, which is the oid of the type.
     * ----------------
     */
    heap_endscan(scan);
    *defined = (bool) ((TypeTupleForm) GETSTRUCT(tup))->typisdefined;

    return
            tup->t_oid;
}

/* ----------------------------------------------------------------
 * 	TypeGet
 *
 *	Finds the ObjectId of a type, even if uncommitted; "defined"
 *	is only set if the type has actually been defined, i.e., if
 *	the type tuple is not a shell.
 *
 *	Note: the meat of this function is now in the function
 *	      TypeGetWithOpenRelation().  -cim 6/15/90
 *
 *	Also called from util/remove.c
 * ----------------------------------------------------------------
 */
Oid
TypeGet(char *typeName,        /* name of type to be fetched */
        bool *defined)        /* has the type been defined? */
{
    Relation pg_type_desc;
    Oid typeoid;

    /* ----------------
     *	open the pg_type relation
     * ----------------
     */
    pg_type_desc = heap_openr(TypeRelationName);

    /* ----------------
     *	scan the type relation for the information we want
     * ----------------
     */
    typeoid = TypeGetWithOpenRelation(pg_type_desc,
                                      typeName,
                                      defined);

    /* ----------------
     *	close the type relation and return the type oid.
     * ----------------
     */
    heap_close(pg_type_desc);

    return
            typeoid;
}

/* ----------------------------------------------------------------
 *	TypeShellMakeWithOpenRelation
 *
 * ----------------------------------------------------------------
 */
Oid
TypeShellMakeWithOpenRelation(Relation pg_type_desc, char *typeName) {
    register int i;
    HeapTuple tup;
    Datum values[Natts_pg_type];
    char nulls[Natts_pg_type];
    Oid typoid;
    TupleDesc tupDesc;

    /* ----------------
     *	initialize our nulls[] and values[] arrays
     * ----------------
     */
    for (i = 0; i < Natts_pg_type; ++i) {
        nulls[i] = ' ';
        values[i] = (Datum) NULL;    /* redundant, but safe */
    }

    /* ----------------
     *	initialize values[] with the type name and 
     * ----------------
     */
    i = 0;
    values[i++] = (Datum) typeName;    /* 1 */
    values[i++] = (Datum) InvalidOid;    /* 2 */
    values[i++] = (Datum) (int16) 0;    /* 3 */
    values[i++] = (Datum) (int16) 0;    /* 4 */
    values[i++] = (Datum) (bool) 0;    /* 5 */
    values[i++] = (Datum) (bool) 0;    /* 6 */
    values[i++] = (Datum) (bool) 0;    /* 7 */
    values[i++] = (Datum) (bool) 0;    /* 8 */
    values[i++] = (Datum) InvalidOid;    /* 9 */
    values[i++] = (Datum) InvalidOid;    /* 10 */
    values[i++] = (Datum) InvalidOid;    /* 11 */
    values[i++] = (Datum) InvalidOid;    /* 12 */
    values[i++] = (Datum) InvalidOid;    /* 13 */
    values[i++] = (Datum) InvalidOid;    /* 14 */
    values[i++] = (Datum) 'i';        /* 15 */

    /*
     * ... and fill typdefault with a bogus value
     */
    values[i++] =
            (Datum) fmgr(TextInRegProcedure, typeName);    /* 15 */

    /* ----------------
     *	create a new type tuple with FormHeapTuple
     * ----------------
     */
    tupDesc = pg_type_desc->rd_att;

    tup = heap_formtuple(tupDesc, values, nulls);

    /* ----------------
     *	insert the tuple in the relation and get the tuple's oid.
     * ----------------
     */
    heap_insert(pg_type_desc, tup);
    typoid = tup->t_oid;

    if (RelationGetRelationTupleForm(pg_type_desc)->relhasindex) {
        Relation idescs[Num_pg_type_indices];

        CatalogOpenIndices(Num_pg_type_indices, Name_pg_type_indices, idescs);
        CatalogIndexInsert(idescs, Num_pg_type_indices, pg_type_desc, tup);
        CatalogCloseIndices(Num_pg_type_indices, idescs);
    }
    /* ----------------
     *	free the tuple and return the type-oid
     * ----------------
     */
    pfree(tup);

    return
            typoid;
}

/* ----------------------------------------------------------------
 *	TypeShellMake
 *
 *	This procedure inserts a "shell" tuple into the type
 *	relation.  The type tuple inserted has invalid values
 *	and in particular, the "typisdefined" field is false.
 *
 *	This is used so that a tuple exists in the catalogs.
 *	The invalid fields should be fixed up sometime after
 *	this routine is called, and then the "typeisdefined"
 *	field is set to true. -cim 6/15/90
 * ----------------------------------------------------------------
 */
Oid
TypeShellMake(char *typeName) {
    Relation pg_type_desc;
    Oid typoid;

    Assert(PointerIsValid(typeName));

    /* ----------------
     *	open pg_type
     * ----------------
     */
    pg_type_desc = heap_openr(TypeRelationName);

    /* ----------------
     *	insert the shell tuple
     * ----------------
     */
    typoid = TypeShellMakeWithOpenRelation(pg_type_desc, typeName);

    /* ----------------
     *	close pg_type and return the tuple's oid.
     * ----------------
     */
    heap_close(pg_type_desc);

    return
            typoid;
}

/* ----------------------------------------------------------------
 *	TypeCreate
 *
 *	This does all the necessary work needed to define a new type.
 * ----------------------------------------------------------------
 */
Oid
TypeCreate(char *typeName,
           Oid relationOid,    /* only for 'c'atalog typeTypes */
           int16 internalSize,
           int16 externalSize,
           char typeType,
           char typDelim,
           char *inputProcedure,
           char *outputProcedure,
           char *sendProcedure,
           char *receiveProcedure,
           char *elementTypeName,
           char *defaultTypeValue, /* internal rep */
           bool passedByValue,
           char alignment) {
    register i, j;
    Relation pg_type_desc;
    HeapScanDesc pg_type_scan;

    Oid typeObjectId;
    Oid elementObjectId = InvalidOid;

    HeapTuple tup;
    char nulls[Natts_pg_type];
    char replaces[Natts_pg_type];
    Datum values[Natts_pg_type];

    Buffer buffer;
    char *procname;
    char *procs[4];
    bool defined;
    ItemPointerData itemPointerData;
    TupleDesc tupDesc;

    Oid argList[8];


    static ScanKeyData typeKey[1] = {
            {0, Anum_pg_type_typname, NameEqualRegProcedure}
    };

    fmgr_info(NameEqualRegProcedure,
              &typeKey[0].sk_func, &typeKey[0].sk_nargs);

    /* ----------------
     *	check that the type is not already defined.
     * ----------------
     */
    typeObjectId = TypeGet(typeName, &defined);
    if (OidIsValid(typeObjectId) && defined) {
        elog(WARN, "TypeCreate: type %s already defined", typeName);
    }

    /* ----------------
     *	if this type has an associated elementType, then we check that
     *  it is defined.
     * ----------------
     */
    if (elementTypeName) {
        elementObjectId = TypeGet(elementTypeName, &defined);
        if (!defined) {
            elog(WARN, "TypeCreate: type %s is not defined", elementTypeName);
        }
    }

    /* ----------------
     *	XXX comment me
     * ----------------
     */
    if (externalSize == 0) {
        externalSize = -1;        /* variable length */
    }

    /* ----------------
     *	initialize arrays needed by FormHeapTuple
     * ----------------
     */
    for (i = 0; i < Natts_pg_type; ++i) {
        nulls[i] = ' ';
        replaces[i] = 'r';
        values[i] = (Datum) NULL;    /* redundant, but nice */
    }

    /*
     * XXX
     *
     * Do this so that user-defined types have size -1 instead of zero if
     * they are variable-length - this is so that everything else in the
     * backend works.
     */

    if (internalSize == 0)
        internalSize = -1;

    /* ----------------
     *	initialize the values[] information
     * ----------------
     */
    i = 0;
    values[i++] = PointerGetDatum(typeName);    /* 1 */
    values[i++] = (Datum) GetUserId();    /* 2 */
    values[i++] = (Datum) internalSize;    /* 3 */
    values[i++] = (Datum) externalSize;    /* 4 */
    values[i++] = (Datum) passedByValue; /* 5 */
    values[i++] = (Datum) typeType;    /* 6 */
    values[i++] = (Datum) (bool) 1;    /* 7 */
    values[i++] = (Datum) typDelim;    /* 8 */
    values[i++] = (Datum) (typeType == 'c' ? relationOid : InvalidOid); /* 9 */
    values[i++] = (Datum) elementObjectId; /* 10 */

    /*
     * arguments to type input and output functions must be 0
     */
    memset(argList, 0, 8 * sizeof(Oid));

    procs[0] = inputProcedure;
    procs[1] = outputProcedure;
    procs[2] = (receiveProcedure) ? receiveProcedure : inputProcedure;
    procs[3] = (sendProcedure) ? sendProcedure : outputProcedure;

    for (j = 0; j < 4; ++j) {
        procname = procs[j];

        tup = SearchSysCacheTuple(PRONAME,
                                  PointerGetDatum(procname),
                                  Int32GetDatum(1),
                                  PointerGetDatum(argList),
                                  0);

        if (!HeapTupleIsValid(tup)) {
            /*
             * it is possible for the input/output procedure
             * to take two arguments, where the second argument
             * is the element type (eg array_in/array_out)
             */
            if (OidIsValid(elementObjectId)) {
                tup = SearchSysCacheTuple(PRONAME,
                                          PointerGetDatum(procname),
                                          Int32GetDatum(2),
                                          PointerGetDatum(argList),
                                          0);
            }
            if (!HeapTupleIsValid(tup)) {
                func_error("TypeCreate", procname, 1, (int *) argList);
            }
        }

        values[i++] = (Datum) tup->t_oid;    /* 11 - 14 */
    }

    /* ----------------
     * set default alignment
     * ----------------
     */
    values[i++] = (Datum) alignment;        /* 15 */

    /* ----------------
     *	initialize the default value for this type.
     * ----------------
     */
    values[i] = (Datum) fmgr(TextInRegProcedure,        /* 16 */
                             PointerIsValid(defaultTypeValue)
                             ? defaultTypeValue : "-");    /* XXX default typdefault */

    /* ----------------
     *	open pg_type and begin a scan for the type name.
     * ----------------
     */
    pg_type_desc = heap_openr(TypeRelationName);

    /* -----------------
     * Set a write lock initially so as not upgrade a read to a write
     * when the heap_insert() or heap_replace() is called.
     * -----------------
     */
    RelationSetLockForWrite(pg_type_desc);

    typeKey[0].sk_argument = PointerGetDatum(typeName);
    pg_type_scan = heap_beginscan(pg_type_desc,
                                  0,
                                  SelfTimeQual,
                                  1,
                                  typeKey);

    /* ----------------
     *  define the type either by adding a tuple to the type
     *  relation, or by updating the fields of the "shell" tuple
     *  already there.
     * ----------------
     */
    tup = heap_getnext(pg_type_scan, 0, &buffer);
    if (HeapTupleIsValid(tup)) {
        tup = heap_modifytuple(tup,
                               buffer,
                               pg_type_desc,
                               values,
                               nulls,
                               replaces);

        /* XXX may not be necessary */
        ItemPointerCopy(&tup->t_ctid, &itemPointerData);

        setheapoverride(true);
        (void) heap_replace(pg_type_desc, &itemPointerData, tup);
        setheapoverride(false);

        typeObjectId = tup->t_oid;
    } else {
        tupDesc = pg_type_desc->rd_att;

        tup = heap_formtuple(tupDesc,
                             values,
                             nulls);

        heap_insert(pg_type_desc, tup);

        typeObjectId = tup->t_oid;
    }

    /* ----------------
     *	finish up
     * ----------------
     */
    heap_endscan(pg_type_scan);

    if (RelationGetRelationTupleForm(pg_type_desc)->relhasindex) {
        Relation idescs[Num_pg_type_indices];

        CatalogOpenIndices(Num_pg_type_indices, Name_pg_type_indices, idescs);
        CatalogIndexInsert(idescs, Num_pg_type_indices, pg_type_desc, tup);
        CatalogCloseIndices(Num_pg_type_indices, idescs);
    }
    RelationUnsetLockForWrite(pg_type_desc);
    heap_close(pg_type_desc);


    return
            typeObjectId;
}

/* ----------------------------------------------------------------
 *	TypeRename
 *
 *	This renames a type 
 * ----------------------------------------------------------------
 */
void
TypeRename(char *oldTypeName, char *newTypeName) {
    Relation pg_type_desc;
    Relation idescs[Num_pg_type_indices];
    Oid type_oid;
    HeapTuple tup;
    bool defined;
    ItemPointerData itemPointerData;

    /* check that that the new type is not already defined */
    type_oid = TypeGet(newTypeName, &defined);
    if (OidIsValid(type_oid) && defined) {
        elog(WARN, "TypeRename: type %s already defined", newTypeName);
    }

    /* get the type tuple from the catalog index scan manager */
    pg_type_desc = heap_openr(TypeRelationName);
    tup = TypeNameIndexScan(pg_type_desc, oldTypeName);

    /* ----------------
     *  change the name of the type
     * ----------------
     */
    if (HeapTupleIsValid(tup)) {

        namestrcpy(&(((TypeTupleForm) GETSTRUCT(tup))->typname), newTypeName);

        ItemPointerCopy(&tup->t_ctid, &itemPointerData);

        setheapoverride(true);
        heap_replace(pg_type_desc, &itemPointerData, tup);
        setheapoverride(false);

        /* update the system catalog indices */
        CatalogOpenIndices(Num_pg_type_indices, Name_pg_type_indices, idescs);
        CatalogIndexInsert(idescs, Num_pg_type_indices, pg_type_desc, tup);
        CatalogCloseIndices(Num_pg_type_indices, idescs);

        /* all done */
        pfree(tup);

    } else {
        elog(WARN, "TypeRename: type %s not defined", oldTypeName);
    }

    /* finish up */
    heap_close(pg_type_desc);
}

/*
 * makeArrayTypeName(typeName);
 *    - given a base type name, make an array of type name out of it
 *
 * the CALLER is responsible for pfreeing the 
 */

char *
makeArrayTypeName(char *typeName) {
    char *arr;

    if (!typeName) return NULL;
    arr = palloc(strlen(typeName) + 2);
    arr[0] = '_';
    strcpy(arr + 1, typeName);

    return arr;

}
