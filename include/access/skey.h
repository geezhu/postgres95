/*-------------------------------------------------------------------------
 *
 * skey.h--
 *    POSTGRES scan key definitions.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: skey.h,v 1.1.1.1 1996/07/09 06:21:09 scrappy Exp $
 *
 *
 * Note:
 *	Needs more accessor/assignment routines.
 *-------------------------------------------------------------------------
 */
#ifndef    SKEY_H
#define SKEY_H

#include "postgres.h"
#include "access/attnum.h"


typedef struct ScanKeyData {
    bits16 sk_flags;    /* flags */
    AttrNumber sk_attno;    /* domain number */
    RegProcedure sk_procedure;    /* procedure OID */
    func_ptr sk_func;
    int32 sk_nargs;
    Datum sk_argument;    /* data to compare */
} ScanKeyData;

typedef ScanKeyData *ScanKey;


#define    SK_ISNULL    0x1
#define    SK_UNARY    0x2
#define    SK_NEGATE    0x4
#define    SK_COMMUTE    0x8

#define ScanUnmarked        0x01
#define ScanUncheckedPrevious    0x02
#define ScanUncheckedNext    0x04


/*
 * prototypes for functions in access/common/scankey.c
 */
extern void ScanKeyEntrySetIllegal(ScanKey entry);

extern void ScanKeyEntryInitialize(ScanKey entry, bits16 flags,
                                   AttrNumber attributeNumber, RegProcedure procedure, Datum argument);

#endif    /* SKEY_H */
