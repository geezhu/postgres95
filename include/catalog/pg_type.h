/*-------------------------------------------------------------------------
 *
 * pg_type.h--
 *    definition of the system "type" relation (pg_type)
 *    along with the relation's initial contents.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_type.h,v 1.1.1.1 1996/07/09 06:21:18 scrappy Exp $
 *
 * NOTES
 *    the genbki.sh script reads this file and generates .bki
 *    information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_TYPE_H
#define PG_TYPE_H

/* ----------------
 *	postgres.h contains the system type definintions and the
 *	CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *	can be read by both genbki.sh and the C compiler.
 * ----------------
 */
#include "postgres.h"
#include "utils/rel.h"        /* for Relation */

/* ----------------
 *	pg_type definition.  cpp turns this into
 *	typedef struct FormData_pg_type
 * ----------------
 */
CATALOG(pg_type) BOOTSTRAP {
    NameData typname;
    Oid typowner;
    int2 typlen;
    int2 typprtlen;
    bool typbyval;
    char typtype;
    bool typisdefined;
    char typdelim;
    Oid typrelid;
    Oid typelem;
    regproc typinput;
    regproc typoutput;
    regproc typreceive;
    regproc typsend;
    char typalign;    /* alignment (c=char, s=short, i=int, d=double) */
    text typdefault;    /* VARIABLE LENGTH FIELD */
} TypeTupleFormData;

/* ----------------
 *	Form_pg_type corresponds to a pointer to a tuple with
 *	the format of pg_type relation.
 * ----------------
 */
typedef TypeTupleFormData *TypeTupleForm;

/* ----------------
 *	compiler constants for pg_type
 * ----------------
 */
#define Natts_pg_type            16
#define Anum_pg_type_typname        1
#define Anum_pg_type_typowner        2
#define Anum_pg_type_typlen        3
#define Anum_pg_type_typprtlen        4
#define Anum_pg_type_typbyval        5
#define Anum_pg_type_typtype        6
#define Anum_pg_type_typisdefined    7
#define Anum_pg_type_typdelim        8
#define Anum_pg_type_typrelid        9
#define Anum_pg_type_typelem        10
#define Anum_pg_type_typinput        11
#define Anum_pg_type_typoutput        12
#define Anum_pg_type_typreceive        13
#define Anum_pg_type_typsend        14
#define Anum_pg_type_typalign        15
#define Anum_pg_type_typdefault        16

/* ----------------
 *	initial contents of pg_type
 * ----------------
 */

/* keep the following ordered by OID so that later changes can be made easier*/

/* OIDS 1 - 99 */
DATA(insert OID = 16(bool PGUID 1 1 t b t \054 0   0 boolin boolout boolin boolout c _null_ ));

#define BOOLOID        16

DATA(insert OID = 17(bytea PGUID - 1 - 1 f b t \054 0  18 byteain byteaout byteain byteaout i _null_ ));
DATA(insert OID = 18( char PGUID 1 1 t b t \054 0   0 charin charout charin charout c _null_ ));

DATA(insert OID = 19(name PGUID NAMEDATALEN NAMEDATALEN f b t \054 0  18 namein nameout namein nameout d _null_ ));
DATA(insert OID = 20(char16 PGUID 16 16 f b t \054 0  18 char16in char16out char16in char16out i _null_ ));
/*DATA(insert OID = 20 (  dt         PGUID  4  10 t b t \054 0   0 dtin dtout dtin dtout i _null_ )); */
DATA(insert OID = 21(int2 PGUID 2 5 t b t \054 0   0 int2in int2out int2in int2out s _null_ ));

#define INT2OID        21

DATA(insert OID = 22(int28 PGUID 16 50 f b t \054 0  21 int28in int28out int28in int28out i _null_ ));

/*
 * XXX -- the implementation of int28's in postgres is a hack, and will
 *	  go away someday.  until that happens, there is a case (in the
 *	  catalog cache management code) where we need to step gingerly
 *	  over piles of int28's on the sidewalk.  in order to do so, we
 *	  need the OID of the int28 tuple from pg_type.
 */

#define INT28OID    22


DATA(insert OID = 23(int4 PGUID 4 10 t b t \054 0   0 int4in int4out int4in int4out i _null_ ));

#define INT4OID        23

DATA(insert OID = 24(regproc PGUID 4 16 t b t \054 0   0 regprocin regprocout regprocin regprocout i _null_ ));
DATA(insert OID = 25(text PGUID - 1 - 1 f b t \054 0  18 textin textout textin textout i _null_ ));
DATA(insert OID = 26(oid PGUID 4 10 t b t \054 0   0 int4in int4out int4in int4out i _null_ ));

#define OIDOID        26

DATA(insert OID = 27(tid PGUID 6 19 f b t \054 0   0 tidin tidout tidin tidout i _null_ ));
DATA(insert OID = 28(xid PGUID 4 12 t b t \054 0   0 xidin xidout xidin xidout i _null_ ));
DATA(insert OID = 29(cid PGUID 2 3 t b t \054 0   0 cidin cidout cidin cidout s _null_ ));
DATA(insert OID = 30(oid8 PGUID 32 89 f b t \054 0  26 oid8in oid8out oid8in oid8out i _null_ ));
DATA(insert OID = 32(SET PGUID - 1 - 1 f r t \054 0  -1 textin textout textin textout i _null_ ));

DATA(insert OID = 71(pg_type PGUID 1 1 t b t \054 71 0 foo bar foo bar c _null_));
DATA(insert OID = 75(pg_attribute PGUID 1 1 t b t \054 75 0 foo bar foo bar c _null_));
DATA(insert OID = 76(pg_demon PGUID 1 1 t b t \054 76 0 foo bar foo bar c _null_));
DATA(insert OID = 80(pg_magic PGUID 1 1 t b t \054 80 0 foo bar foo bar c _null_));
DATA(insert OID = 81(pg_proc PGUID 1 1 t b t \054 81 0 foo bar foo bar c _null_));
DATA(insert OID = 82(pg_server PGUID 1 1 t b t \054 82 0 foo bar foo bar c _null_));
DATA(insert OID = 83(pg_class PGUID 1 1 t b t \054 83 0 foo bar foo bar c _null_));
DATA(insert OID = 86(pg_user PGUID 1 1 t b t \054 86 0 foo bar foo bar c _null_));
DATA(insert OID = 87(pg_group PGUID 1 1 t b t \054 87 0 foo bar foo bar c _null_));
DATA(insert OID = 88(pg_database PGUID 1 1 t b t \054 88 0 foo bar foo bar c _null_));
DATA(insert OID = 89(pg_defaults PGUID 1 1 t b t \054 89 0 foo bar foo bar c _null_));
DATA(insert OID = 90(pg_variable PGUID 1 1 t b t \054 90 0 foo bar foo bar c _null_));
DATA(insert OID = 99(pg_log PGUID 1 1 t b t \054 99 0 foo bar foo bar c _null_));

/* OIDS 100 - 199 */

DATA(insert OID = 100(pg_time PGUID 1 1 t b t \054 100 0 foo bar foo bar c _null_));
DATA(insert OID = 101(pg_time PGUID 1 1 t b t \054 101 0 foo bar foo bar c _null_));

/* OIDS 200 - 299 */

DATA(insert OID = 210(smgr PGUID 2 12 t b t \054 0  -1 smgrin smgrout smgrin smgrout s _null_ ));

/* OIDS 300 - 399 */

/* OIDS 400 - 499 */
DATA(insert OID = 409(char2 PGUID 2 2 t b t \054 0  18 char2in char2out char2in char2out s _null_ ));
DATA(insert OID = 410(char4 PGUID 4 4 t b t \054 0  18 char4in char4out char4in char4out i _null_ ));
DATA(insert OID = 411(char8 PGUID 8 8 f b t \054 0  18 char8in char8out char8in char8out i _null_ ));

/* OIDS 500 - 599 */

/* OIDS 600 - 699 */
DATA(insert OID = 600(point PGUID 16 24 f b t \054 0 701 point_in point_out point_in point_out d _null_ ));
DATA(insert OID = 601(lseg PGUID 32 48 f b t \054 0 600 lseg_in lseg_out lseg_in lseg_out d _null_ ));
DATA(insert OID = 602(path PGUID - 1 - 1 f b t \054 0 600 path_in path_out path_in path_out d _null_ ));
DATA(insert OID = 603(box PGUID 32 100 f b t \073 0 600 box_in box_out box_in box_out d _null_ ));
DATA(insert OID = 604(polygon PGUID - 1 - 1 f b t \054 0  -1 poly_in poly_out poly_in poly_out d _null_ ));
DATA(insert OID = 605(filename PGUID 256 -
                                     1 f b t \054 0 18 filename_in filename_out filename_in filename_out i _null_ ));

/* OIDS 700 - 799 */

#define FLOAT4OID 700

DATA(insert OID = 700(float4 PGUID 4 12 f b t \054 0   0 float4in float4out float4in float4out i _null_ ));


#define FLOAT8OID 701

DATA(insert OID = 701(float8 PGUID 8 24 f b t \054 0   0 float8in float8out float8in float8out d _null_ ));
DATA(insert OID = 702(abstime PGUID 4 20 t b t \054 0   0 nabstimein nabstimeout nabstimein nabstimeout i _null_ ));
DATA(insert OID = 703(reltime PGUID 4 20 t b t \054 0   0 reltimein reltimeout reltimein reltimeout i _null_ ));
DATA(insert OID = 704(
        tinterval PGUID 12 47 f b t \054 0   0 tintervalin tintervalout tintervalin tintervalout i _null_ ));
DATA(insert OID = 705(unknown PGUID - 1 - 1 f b t \054 0   18 textin textout textin textout i _null_ ));

#define UNKNOWNOID    705

/* OIDS 800 - 899 */
DATA(insert OID = 810(oidint2 PGUID 6 20 f b t \054 0   0 oidint2in oidint2out oidint2in oidint2out i _null_ ));

/* OIDS 900 - 999 */
DATA(insert OID = 910(oidint4 PGUID 8 20 f b t \054 0   0 oidint4in oidint4out oidint4in oidint4out i _null_ ));
DATA(insert OID = 911(
        oidname PGUID OIDNAMELEN OIDNAMELEN f b t \054 0   0 oidnamein oidnameout oidnamein oidnameout i _null_ ));

/* OIDS 1000 - 1099 */
DATA(insert OID = 1000(_bool PGUID - 1 - 1 f b t \054 0  16 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1001(_bytea PGUID - 1 - 1 f b t \054 0  17 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1002(_char PGUID - 1 - 1 f b t \054 0  18 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1003(_name PGUID - 1 - 1 f b t \054 0  19 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1004(_char16 PGUID - 1 - 1 f b t \054 0  19 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1005(_int2 PGUID - 1 - 1 f b t \054 0  21 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1006(_int28 PGUID - 1 - 1 f b t \054 0  22 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1007(_int4 PGUID - 1 - 1 f b t \054 0  23 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1008(_regproc PGUID - 1 - 1 f b t \054 0  24 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1009(_text PGUID - 1 - 1 f b t \054 0  25 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1028(_oid PGUID - 1 - 1 f b t \054 0  26 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1010(_tid PGUID - 1 - 1 f b t \054 0  27 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1011(_xid PGUID - 1 - 1 f b t \054 0  28 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1012(_cid PGUID - 1 - 1 f b t \054 0  29 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1013(_oid8 PGUID - 1 - 1 f b t \054 0  30 array_in array_out array_in array_out i _null_ ));
/*DATA(insert OID = 1014 (  _lock      PGUID -1  -1 f b t \054 0  31 array_in array_out array_in array_out i _null_ ));*/
DATA(insert OID = 1015(_stub PGUID - 1 - 1 f b t \054 0  33 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1016(_ref PGUID - 1 - 1 f b t \054 0 591 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1017(_point PGUID - 1 - 1 f b t \054 0 600 array_in array_out array_in array_out d _null_ ));
DATA(insert OID = 1018(_lseg PGUID - 1 - 1 f b t \054 0 601 array_in array_out array_in array_out d _null_ ));
DATA(insert OID = 1019(_path PGUID - 1 - 1 f b t \054 0 602 array_in array_out array_in array_out d _null_ ));
DATA(insert OID = 1020(_box PGUID - 1 - 1 f b t \073 0 603 array_in array_out array_in array_out d _null_ ));
DATA(insert OID = 1021(_float4 PGUID - 1 - 1 f b t \054 0 700 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1022(_float8 PGUID - 1 - 1 f b t \054 0 701 array_in array_out array_in array_out d _null_ ));
DATA(insert OID = 1023(_abstime PGUID - 1 - 1 f b t \054 0 702 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1024(_reltime PGUID - 1 - 1 f b t \054 0 703 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1025(_tinterval PGUID - 1 - 1 f b t \054 0 704 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1026(_filename PGUID - 1 - 1 f b t \054 0 605 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1027(_polygon PGUID - 1 - 1 f b t \054 0 604 array_in array_out array_in array_out d _null_ ));
/* Note: the size of an aclitem needs to match sizeof(AclItem) in acl.h */
DATA(insert OID = 1033(aclitem PGUID 8 - 1 f b t \054 0 0 aclitemin aclitemout aclitemin aclitemout i _null_ ));
DATA(insert OID = 1034(_aclitem PGUID - 1 - 1 f b t \054 0 1033 array_in array_out array_in array_out i _null_ ));

DATA(insert OID = 1039(_char2 PGUID - 1 - 1 f b t \054 0  409 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1040(_char4 PGUID - 1 - 1 f b t \054 0  410 array_in array_out array_in array_out i _null_ ));
DATA(insert OID = 1041(_char8 PGUID - 1 - 1 f b t \054 0  411 array_in array_out array_in array_out i _null_ ));

#define    BPCHAROID    1042
DATA(insert OID = 1042(bpchar PGUID - 1 - 1 f b t \054 0  18 bpcharin bpcharout bpcharin bpcharout i _null_ ));
#define    VARCHAROID    1043
DATA(insert OID = 1043(varchar PGUID - 1 - 1 f b t \054 0  18 varcharin varcharout varcharin varcharout i _null_ ));

DATA(insert OID = 1082(date PGUID 4 10 t b t \054 0  0 date_in date_out date_in date_out i _null_ ));
DATA(insert OID = 1083(time PGUID 8 16 f b t \054 0  0 time_in time_out time_in time_out i _null_ ));

/*
 * prototypes for functions in pg_type.c 
 */
extern Oid TypeGet(char *typeName, bool *defined);

extern Oid TypeShellMakeWithOpenRelation(Relation pg_type_desc,
                                         char *typeName);

extern Oid TypeShellMake(char *typeName);

extern Oid TypeCreate(char *typeName,
                      Oid relationOid,
                      int16 internalSize,
                      int16 externalSize,
                      char typeType,
                      char typDelim,
                      char *inputProcedure,
                      char *outputProcedure,
                      char *sendProcedure,
                      char *receiveProcedure,
                      char *elementTypeName,
                      char *defaultTypeValue,
                      bool passedByValue, char alignment);

extern void TypeRename(char *oldTypeName, char *newTypeName);

extern char *makeArrayTypeName(char *typeName);


#endif /* PG_TYPE_H */
