/*-------------------------------------------------------------------------
 *
 * pg_server.h--
 *    definition of the system "server" relation (pg_server)
 *    along with the relation's initial contents.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_server.h,v 1.1.1.1 1996/07/09 06:21:18 scrappy Exp $
 *
 * NOTES
 *    the genbki.sh script reads this file and generates .bki
 *    information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_SERVER_H
#define PG_SERVER_H

/* ----------------
 *	postgres.h contains the system type definintions and the
 *	CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *	can be read by both genbki.sh and the C compiler.
 * ----------------
 */
#include "postgres.h"

/* ----------------
 *	pg_server definition.  cpp turns this into
 *	typedef struct FormData_pg_server
 * ----------------
 */
CATALOG(pg_server) BOOTSTRAP {
    NameData sername;
    int2 serpid;
    int2 serport;
} FormData_pg_server;

/* ----------------
 *	Form_pg_server corresponds to a pointer to a tuple with
 *	the format of pg_server relation.
 * ----------------
 */
typedef FormData_pg_server *Form_pg_server;

/* ----------------
 *	compiler constants for pg_server
 * ----------------
 */
#define Natts_pg_server            3
#define Anum_pg_server_sername        1
#define Anum_pg_server_serpid        2
#define Anum_pg_server_serport        3

#endif /* PG_SERVER_H */
