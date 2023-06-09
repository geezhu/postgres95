/*-------------------------------------------------------------------------
 *
 * pg_time.h--
 *    the system commit-time relation "pg_time" is not a "heap" relation.
 *    it is automatically created by the transam/ code and the
 *    information here is all bogus and is just here to make the
 *    relcache code happy.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_time.h,v 1.1.1.1 1996/07/09 06:21:18 scrappy Exp $
 *
 * NOTES
 *    The structures and macros used by the transam/ code
 *    to access pg_time should some day go here -cim 6/18/90
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_TIME_H
#define PG_TIME_H

/* ----------------
 *	postgres.h contains the system type definintions and the
 *	CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *	can be read by both genbki.sh and the C compiler.
 * ----------------
 */
#include "postgres.h"

CATALOG(pg_time) BOOTSTRAP {
    Oid timefoo;
} FormData_pg_time;

typedef FormData_pg_time *Form_pg_time;

#define Natts_pg_time        1
#define Anum_pg_time_timefoo    1


#endif /* PG_TIME_H */
