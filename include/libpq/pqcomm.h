/*-------------------------------------------------------------------------
 *
 * pqcomm.h--
 *    Parameters for the communication module
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pqcomm.h,v 1.1.1.1 1996/07/09 06:21:31 scrappy Exp $
 *
 * NOTES
 *    Some of this should move to libpq.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PQCOMM_H
#define    PQCOMM_H

#include <sys/types.h>

#ifdef WIN32
#include <winsock.h>
#else

#include <netinet/in.h>

#endif /* WIN32 */

#include "postgres.h"

/*
 * startup msg parameters: path length, argument string length
 */
#define PATH_SIZE    64
#define ARGV_SIZE    64


typedef enum _MsgType {
    ACK_MSG = 0,        /* acknowledge a message */
    ERROR_MSG = 1,        /* error response to client from server */
    RESET_MSG = 2,        /* client must reset connection */
    PRINT_MSG = 3,        /* tuples for client from server */
    NET_ERROR = 4,        /* error in net system call */
    FUNCTION_MSG = 5,        /* fastpath call (unused) */
    QUERY_MSG = 6,        /* client query to server */
    STARTUP_MSG = 7,        /* initialize a connection with a backend */
    DUPLICATE_MSG = 8,        /* duplicate msg arrived (errors msg only) */
    INVALID_MSG = 9,        /* for some control functions */
    STARTUP_KRB4_MSG = 10,    /* krb4 session follows startup packet */
    STARTUP_KRB5_MSG = 11,        /* krb5 session follows startup packet */
    STARTUP_HBA_MSG = 12         /* use host-based authentication */
    /* insert new values here -- DO NOT REORDER OR DELETE ENTRIES */
} MsgType;

typedef char *Addr;
typedef int PacketLen;    /* packet length */


typedef struct StartupInfo {
/*     PacketHdr	hdr; */
    char database[PATH_SIZE]; /* database name */
    char user[NAMEDATALEN];   /* user name */
    char options[ARGV_SIZE];  /* possible additional args */
    char execFile[ARGV_SIZE]; /* possible backend to use */
    char tty[PATH_SIZE];         /* possible tty for debug output*/
} StartupInfo;

/* amount of available data in a packet buffer */
#define MESSAGE_SIZE    sizeof(StartupInfo) + 5

/* I/O can be blocking or non-blocking */
#define BLOCKING    (FALSE)
#define NON_BLOCKING    (TRUE)

/* a PacketBuf gets shipped from client to server so be careful
   of differences in representation.  
   Be sure to use htonl() and ntohl() on the len and msgtype fields! */
typedef struct PacketBuf {
    int len;
    MsgType msgtype;
    char data[MESSAGE_SIZE];
} PacketBuf;

/* update the conversion routines 
  StartupInfo2PacketBuf() and PacketBuf2StartupInfo() (decl. below)
  if StartupInfo or PacketBuf structs ever change */

/*
 * socket descriptor port 
 *	we need addresses of both sides to do authentication calls
 */
typedef struct Port {
    int sock;    /* file descriptor */
    int mask;    /* select mask */
    int nBytes;    /* nBytes read in so far */
    struct sockaddr_in laddr;    /* local addr (us) */
    struct sockaddr_in raddr;    /* remote addr (them) */
/*     PacketBufId		id;*/    /* id of packet buf currently in use */
    PacketBuf buf;    /* stream implementation (curr pack buf) */
} Port;

/* invalid socket descriptor */
#define INVALID_SOCK    (-1)

#define INVALID_ID (-1)
#define MAX_CONNECTIONS    10
#define N_PACK_BUFS    20

/* no multi-packet messages yet */
#define MAX_PACKET_BACKLOG    1

#define    DEFAULT_STRING        ""

extern FILE *Pfout, *Pfin;
extern int PQAsyncNotifyWaiting;

/*
 * prototypes for functions in pqpacket.c
 */
extern int PacketReceive(Port *port, PacketBuf *buf, bool nonBlocking);

extern int PacketSend(Port *port, PacketBuf *buf,
                      PacketLen len, bool nonBlocking);
/* extern PacketBuf* StartupInfo2PacketBuf(StartupInfo*); */
/* extern StartupInfo* PacketBuf2StartupInfo(PacketBuf*); */


#endif    /* PQCOMM_H */
