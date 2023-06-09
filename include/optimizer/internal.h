/*-------------------------------------------------------------------------
 *
 * internal.h--
 *    Definitions required throughout the query optimizer.
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: internal.h,v 1.1.1.1 1996/07/09 06:21:34 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef INTERNAL_H
#define INTERNAL_H

/*    
 *    	---------- SHARED MACROS
 *    
 *     	Macros common to modules for creating, accessing, and modifying
 *    	query tree and query plan components.
 *    	Shared with the executor.
 *    
 */

#include "nodes/nodes.h"
#include "nodes/primnodes.h"
#include "nodes/plannodes.h"
#include "parser/parsetree.h"
#include "nodes/relation.h"
#include "catalog/pg_index.h"    /* for INDEX_MAX_KEYS */
#include "utils/syscache.h"    /* for SearchSysCacheGetAttribute, etc. */

/*
 *    	System-dependent tuning constants
 *    
 */
#define _CPU_PAGE_WEIGHT_  0.065      /* CPU-to-page cost weighting factor */
#define _PAGE_SIZE_    8192           /* BLCKSZ (from ../h/bufmgr.h) */
#define _MAX_KEYS_     INDEX_MAX_KEYS /* maximum number of keys in an index */
#define _TID_SIZE_     6              /* sizeof(itemid) (from ../h/itemid.h) */

/*    
 *    	Size estimates
 *    
 */

/*     The cost of sequentially scanning a materialized temporary relation
 */
#define _TEMP_SCAN_COST_    10

/*     The number of pages and tuples in a materialized relation
 */
#define _TEMP_RELATION_PAGES_        1
#define _TEMP_RELATION_TUPLES_    10

/*     The length of a variable-length field in bytes
 */
#define _DEFAULT_ATTRIBUTE_WIDTH_ (2 * _TID_SIZE_)

/*    
 *    	Flags and identifiers
 *    
 */

/*     Identifier for (sort) temp relations   */
/* used to be -1 */
#define _TEMP_RELATION_ID_   InvalidOid

/*     Identifier for invalid relation OIDs and attribute numbers for use by
 *     selectivity functions
 */
#define _SELEC_VALUE_UNKNOWN_   -1

/*     Flag indicating that a clause constant is really a parameter (or other 
 *     	non-constant?), a non-parameter, or a constant on the right side
 *    	of the clause.
 */
#define _SELEC_NOT_CONSTANT_   0
#define _SELEC_IS_CONSTANT_    1
#define _SELEC_CONSTANT_LEFT_  0
#define _SELEC_CONSTANT_RIGHT_ 2

#define TOLERANCE 0.000001

#define FLOAT_EQUAL(X, Y) ((X) - (Y) < TOLERANCE)
#define FLOAT_IS_ZERO(X) (FLOAT_EQUAL(X,0.0))

extern int BushyPlanFlag;
/* #define deactivate_joininfo(joininfo)	joininfo->inactive=true*/
/*#define joininfo_inactive(joininfo)	joininfo->inactive */

#endif    /* INTERNAL_H */
