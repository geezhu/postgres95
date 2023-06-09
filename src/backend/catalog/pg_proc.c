/*-------------------------------------------------------------------------
 *
 * pg_proc.c--
 *    routines to support manipulation of the pg_proc relation
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *    $Header: /cvsroot/pgsql/src/backend/catalog/pg_proc.c,v 1.1.1.1 1996/07/09 06:21:17 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <string.h>
#include "postgres.h"

#include "access/heapam.h"
#include "access/relscan.h"
#include "access/skey.h"
#include "utils/rel.h"
#include "fmgr.h"
#include "utils/elog.h"
#include "utils/builtins.h"
#include "utils/sets.h"

#include "nodes/pg_list.h"

#include "catalog/catname.h"
#include "utils/syscache.h"
#include "catalog/pg_proc.h"
#include "catalog/indexing.h"
#include "tcop/dest.h"
#include "parser/parse_query.h"
#include "tcop/tcopprot.h"
#include "catalog/pg_type.h"
#include "parser/catalog_utils.h"
#include "utils/lsyscache.h"
#include "optimizer/internal.h"
#include "optimizer/planner.h"

/* ----------------------------------------------------------------
 *	ProcedureDefine
 * ----------------------------------------------------------------
 */
Oid
ProcedureCreate(char *procedureName,
                bool returnsSet,
                char *returnTypeName,
                char *languageName,
                char *prosrc,
                char *probin,
                bool canCache,
                bool trusted,
                int32 byte_pct,
                int32 perbyte_cpu,
                int32 percall_cpu,
                int32 outin_ratio,
                List *argList,
                CommandDest dest) {
    register i;
    Relation rdesc;
    HeapTuple tup;
    bool defined;
    uint16 parameterCount;
    char nulls[Natts_pg_proc];
    Datum values[Natts_pg_proc];
    Oid languageObjectId;
    Oid typeObjectId;
    List *x;
    QueryTreeList *querytree_list;
    List *plan_list;
    Oid typev[8];
    Oid relid;
    Oid toid;
    text *prosrctext;
    TupleDesc tupDesc;

    /* ----------------
     *	sanity checks
     * ----------------
     */
    Assert(PointerIsValid(prosrc));
    Assert(PointerIsValid(probin));

    parameterCount = 0;
    memset(typev, 0, 8 * sizeof(Oid));
    foreach (x, argList) {
        Value *t = lfirst(x);

        if (parameterCount == 8)
            elog(WARN, "Procedures cannot take more than 8 arguments");

        if (strcmp(strVal(t), "opaque") == 0) {
            if (strcmp(languageName, "sql") == 0) {
                elog(WARN, "ProcedureDefine: sql functions cannot take type \"opaque\"");
            } else
                toid = 0;
        } else {
            toid = TypeGet(strVal(t), &defined);

            if (!OidIsValid(toid)) {
                elog(WARN, "ProcedureCreate: arg type '%s' is not defined",
                     strVal(t));
            }

            if (!defined) {
                elog(NOTICE, "ProcedureCreate: arg type '%s' is only a shell",
                     strVal(t));
            }
        }

        typev[parameterCount++] = toid;
    }

    tup = SearchSysCacheTuple(PRONAME,
                              PointerGetDatum(procedureName),
                              UInt16GetDatum(parameterCount),
                              PointerGetDatum(typev),
                              0);

    if (HeapTupleIsValid(tup))
        elog(WARN, "ProcedureCreate: procedure %s already exists with same arguments",
             procedureName);

    if (!strcmp(languageName, "sql")) {
        /* If this call is defining a set, check if the set is already
         * defined by looking to see whether this call's function text
         * matches a function already in pg_proc.  If so just return the 
         * OID of the existing set.
         */
        if (!strcmp(procedureName, GENERICSETNAME)) {
            prosrctext = textin(prosrc);
            tup = SearchSysCacheTuple(PROSRC,
                                      PointerGetDatum(prosrctext),
                                      0, 0, 0);
            if (HeapTupleIsValid(tup))
                return tup->t_oid;
        }
    }

    tup = SearchSysCacheTuple(LANNAME,
                              PointerGetDatum(languageName),
                              0, 0, 0);

    if (!HeapTupleIsValid(tup))
        elog(WARN, "ProcedureCreate: no such language %s",
             languageName);

    languageObjectId = tup->t_oid;

    if (strcmp(returnTypeName, "opaque") == 0) {
        if (strcmp(languageName, "sql") == 0) {
            elog(WARN, "ProcedureCreate: sql functions cannot return type \"opaque\"");
        } else
            typeObjectId = 0;
    } else {
        typeObjectId = TypeGet(returnTypeName, &defined);

        if (!OidIsValid(typeObjectId)) {
            elog(NOTICE, "ProcedureCreate: type '%s' is not yet defined",
                 returnTypeName);
#if 0
            elog(NOTICE, "ProcedureCreate: creating a shell for type '%s'",
             returnTypeName);
#endif
            typeObjectId = TypeShellMake(returnTypeName);
            if (!OidIsValid(typeObjectId)) {
                elog(WARN, "ProcedureCreate: could not create type '%s'",
                     returnTypeName);
            }
        } else if (!defined) {
            elog(NOTICE, "ProcedureCreate: return type '%s' is only a shell",
                 returnTypeName);
        }
    }

    /* don't allow functions of complex types that have the same name as
       existing attributes of the type */
    if (parameterCount == 1 &&
        (toid = TypeGet(strVal(lfirst(argList)), &defined)) &&
        defined &&
        (relid = typeid_get_relid(toid)) != 0 &&
        get_attnum(relid, procedureName) != InvalidAttrNumber)
        elog(WARN, "method %s already an attribute of type %s",
             procedureName, strVal(lfirst(argList)));


    /*
     *  If this is a postquel procedure, we parse it here in order to
     *  be sure that it contains no syntax errors.  We should store
     *  the plan in an Inversion file for use later, but for now, we
     *  just store the procedure's text in the prosrc attribute.
     */

    if (strcmp(languageName, "sql") == 0) {
        plan_list = pg_plan(prosrc, typev, parameterCount,
                            &querytree_list, dest);

        /* typecheck return value */
        pg_checkretval(typeObjectId, querytree_list);
    }

    for (i = 0; i < Natts_pg_proc; ++i) {
        nulls[i] = ' ';
        values[i] = (Datum) NULL;
    }

    i = 0;
    values[i++] = PointerGetDatum(procedureName);
    values[i++] = Int32GetDatum(GetUserId());
    values[i++] = ObjectIdGetDatum(languageObjectId);

    /* XXX isinherited is always false for now */

    values[i++] = Int8GetDatum((bool) 0);

    /* XXX istrusted is always false for now */

    values[i++] = Int8GetDatum(trusted);
    values[i++] = Int8GetDatum(canCache);
    values[i++] = UInt16GetDatum(parameterCount);
    values[i++] = Int8GetDatum(returnsSet);
    values[i++] = ObjectIdGetDatum(typeObjectId);

    values[i++] = (Datum) typev;
    /*
     * The following assignments of constants are made.  The real values
     * will have to be extracted from the arglist someday soon.
     */
    values[i++] = Int32GetDatum(byte_pct); /* probyte_pct */
    values[i++] = Int32GetDatum(perbyte_cpu); /* properbyte_cpu */
    values[i++] = Int32GetDatum(percall_cpu); /* propercall_cpu */
    values[i++] = Int32GetDatum(outin_ratio); /* prooutin_ratio */

    values[i++] = (Datum) fmgr(TextInRegProcedure, prosrc);    /* prosrc */
    values[i++] = (Datum) fmgr(TextInRegProcedure, probin);   /* probin */

    rdesc = heap_openr(ProcedureRelationName);

    tupDesc = rdesc->rd_att;
    tup = heap_formtuple(tupDesc,
                         values,
                         nulls);

    heap_insert(rdesc, tup);

    if (RelationGetRelationTupleForm(rdesc)->relhasindex) {
        Relation idescs[Num_pg_proc_indices];

        CatalogOpenIndices(Num_pg_proc_indices, Name_pg_proc_indices, idescs);
        CatalogIndexInsert(idescs, Num_pg_proc_indices, rdesc, tup);
        CatalogCloseIndices(Num_pg_proc_indices, idescs);
    }
    heap_close(rdesc);
    return tup->t_oid;
}

