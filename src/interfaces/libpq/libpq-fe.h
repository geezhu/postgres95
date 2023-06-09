/*-------------------------------------------------------------------------
 *
 * libpq-fe.h--
 *    This file contains definitions for structures and
 *    externs for functions used by frontend postgres applications.
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: libpq-fe.h,v 1.1.1.1 1996/07/09 06:22:17 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef LIBPQ_FE_H
#define LIBPQ_FE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------
 *	include stuff common to fe and be
 * ----------------
 */
/* #include "libpq/libpq.h" */
#include "libpq/pqcomm.h"
#include "lib/dllist.h"

typedef enum {
    CONNECTION_OK,
    CONNECTION_BAD
} ConnStatusType;

typedef enum {
    PGRES_EMPTY_QUERY = 0,
    PGRES_COMMAND_OK,  /* a query command that doesn't return */
    /* anything was executed properly by the backend */
    PGRES_TUPLES_OK,  /* a query command that returns tuples */
    /* was executed properly by the backend, PGresult */
    /* contains the resulttuples */
    PGRES_COPY_OUT,
    PGRES_COPY_IN,
    PGRES_BAD_RESPONSE, /* an unexpected response was recv'd from the backend */
    PGRES_NONFATAL_ERROR,
    PGRES_FATAL_ERROR

} ExecStatusType;

/* string descriptions of the ExecStatusTypes */
extern char *pgresStatus[];

/* 
 * POSTGRES backend dependent Constants. 
 */

/* ERROR_MSG_LENGTH should really be the same as ELOG_MAXLEN in utils/elog.h*/
#define ERROR_MSG_LENGTH 4096
#define COMMAND_LENGTH 20
#define REMARK_LENGTH 80
#define PORTAL_NAME_LENGTH 16

/* ----------------
 * PQArgBlock --
 *	Information (pointer to array of this structure) required
 *	for the PQfn() call.
 * ----------------
 */
typedef struct {
    int len;
    int isint;
    union {
        int *ptr;    /* can't use void (dec compiler barfs)	*/
        int integer;
    } u;
} PQArgBlock;

typedef struct pgresAttDesc {
    char *name; /* type name */
    Oid adtid;  /* type id */
    int2 adtsize; /* type size */
} PGresAttDesc;

/* use char* for Attribute values,
   ASCII tuples are guaranteed to be null-terminated
   For binary tuples, the first four bytes of the value is the size,
   and the bytes afterwards are the value.  The binary value is 
   not guaranteed to be null-terminated.  In fact, it can have embedded nulls*/
typedef struct pgresAttValue {
    int len; /* length in bytes of the value */
    char *value; /* actual value */
} PGresAttValue;

typedef struct pgNotify {
    char relname[NAMEDATALEN];    /* name of relation containing data */
    int be_pid;            /* process id of backend */
} PGnotify;

/* PGconn encapsulates a connection to the backend */
typedef struct pg_conn {
    char *pghost; /* the machine on which the server is running */
    char *pgtty;  /* tty on which the backend messages is displayed */
    char *pgport; /* the communication port with the backend */
    char *pgoptions; /* options to start the backend with */
    char *dbName; /* database name */
    ConnStatusType status;
    char errorMessage[ERROR_MSG_LENGTH];
    /* pipes for be/fe communication */
    FILE *Pfin;
    FILE *Pfout;
    FILE *Pfdebug;
    void *port; /* really a Port* */
    int asyncNotifyWaiting;
    Dllist *notifyList;
} PGconn;

#define CMDSTATUS_LEN 40

/* PGresult encapsulates the result of a query */
/* unlike the old libpq, we assume that queries only return in one group */
typedef struct pg_result {
    int ntups;
    int numAttributes;
    PGresAttDesc *attDescs;
    PGresAttValue **tuples; /* each PGresTuple is an array of PGresAttValue's */
    int tupArrSize;         /* size of tuples array allocated */
    ExecStatusType resultStatus;
    char cmdStatus[CMDSTATUS_LEN]; /* cmd status from the last insert query*/
    int binary; /* binary tuple values if binary == 1, otherwise ASCII */
    PGconn *conn;
} PGresult;


/* ===  in fe-connect.c === */
/* make a new client connection to the backend */
extern PGconn *PQsetdb(char *pghost, char *pgport, char *pgoptions,
                       char *pgtty, char *dbName);

/* close the current connection and free the PGconn data structure */
extern void PQfinish(PGconn *conn);

/* close the current connection and restablish a new one with the same 
   parameters */
extern void PQreset(PGconn *conn);

extern char *PQdb(PGconn *conn);

extern char *PQhost(PGconn *conn);

extern char *PQoptions(PGconn *conn);

extern char *PQport(PGconn *conn);

extern char *PQtty(PGconn *conn);

extern ConnStatusType PQstatus(PGconn *conn);

extern char *PQerrorMessage(PGconn *conn);

extern void PQtrace(PGconn *conn, FILE *debug_port);

extern void PQuntrace(PGconn *conn);

/* === in fe-exec.c === */
extern PGresult *PQexec(PGconn *conn, char *query);

extern int PQgetline(PGconn *conn, char *string, int length);

extern int PQendcopy(PGconn *conn);

extern void PQputline(PGconn *conn, char *string);

extern ExecStatusType PQresultStatus(PGresult *res);

extern int PQntuples(PGresult *res);

extern int PQnfields(PGresult *res);

extern char *PQfname(PGresult *res, int field_num);

extern int PQfnumber(PGresult *res, char *field_name);

extern Oid PQftype(PGresult *res, int field_num);

extern int2 PQfsize(PGresult *res, int field_num);

extern char *PQcmdStatus(PGresult *res);

extern char *PQoidStatus(PGresult *res);

extern char *PQgetvalue(PGresult *res, int tup_num, int field_num);

extern int PQgetlength(PGresult *res, int tup_num, int field_num);

extern void PQclear(PGresult *res);

/* PQdisplayTuples() is a better version of PQprintTuples() */
extern void PQdisplayTuples(PGresult *res,
                            FILE *fp,      /* where to send the output */
                            int fillAlign, /* pad the fields with spaces */
                            char *fieldSep,  /* field separator */
                            int printHeader, /* display headers? */
                            int quiet);

extern void PQprintTuples(PGresult *res,
                          FILE *fout,      /* output stream */
                          int printAttName,/* print attribute names or not*/
                          int terseOutput, /* delimiter bars or not?*/
                          int width        /* width of column, 
					      if 0, use variable width */
);

extern PGnotify *PQnotifies(PGconn *conn);

extern PGresult *PQfn(PGconn *conn,
                      int fnid,
                      int *result_buf,
                      int *result_len,
                      int result_is_int,
                      PQArgBlock *args,
                      int nargs);

/* === in fe-auth.c === */
extern MsgType fe_getauthsvc(char *PQerrormsg);

extern void fe_setauthsvc(char *name, char *PQerrormsg);

extern char *fe_getauthname(char *PQerrormsg);

/* === in fe-misc.c === */
/* pqGets and pqPuts gets and sends strings to the file stream
   returns 0 if successful 
   if debug is non-null, debugging output is sent to that stream 
*/
extern int pqGets(char *s, int maxlen, FILE *stream, FILE *debug);

extern int pqGetnchar(char *s, int maxlen, FILE *stream, FILE *debug);

extern int pqPutnchar(char *s, int maxlen, FILE *stream, FILE *debug);

extern int pqPuts(char *s, FILE *stream, FILE *debug);

extern int pqGetc(FILE *stream, FILE *debug);
/* get a n-byte integer from the stream into result */
/* returns 0 if successful */
extern int pqGetInt(int *result, int bytes, FILE *stream, FILE *debug);
/* put a n-byte integer into the stream */
/* returns 0 if successful */
extern int pqPutInt(int n, int bytes, FILE *stream, FILE *debug);

extern void pqFlush(FILE *stream, FILE *debug);

/* === in fe-lobj.c === */
int lo_open(PGconn *conn, Oid lobjId, int mode);

int lo_close(PGconn *conn, int fd);

int lo_read(PGconn *conn, int fd, char *buf, int len);

int lo_write(PGconn *conn, int fd, char *buf, int len);

int lo_lseek(PGconn *conn, int fd, int offset, int whence);

Oid lo_creat(PGconn *conn, int mode);

int lo_tell(PGconn *conn, int fd);

int lo_unlink(PGconn *conn, Oid lobjId);

Oid lo_import(PGconn *conn, char *filename);

int lo_export(PGconn *conn, Oid lobjId, char *filename);
/* max length of message to send  */
#define MAX_MESSAGE_LEN 8193

/* maximum number of fields in a tuple */
#define BYTELEN 8
#define MAX_FIELDS 512

/* fall back options if they are not specified by arguments or defined
   by environment variables */
#define DefaultHost    "localhost"
#define DefaultTty    ""
#define DefaultOption    ""

typedef void *TUPLE;
#define palloc malloc
#define pfree free

#if defined(PORTNAME_sparc)
extern char *sys_errlist[];
#define strerror(A) (sys_errlist[(A)])
#endif /* PORTNAME_sparc */

#ifdef __cplusplus
};
#endif

#endif /* LIBPQ_FE_H */

