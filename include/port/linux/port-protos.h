/*-------------------------------------------------------------------------
 *
 * port-protos.h--
 *    port-specific prototypes for SunOS 4
 *
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * $Id: port-protos.h,v 1.1.1.1 1996/07/09 06:21:44 scrappy Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PORT_PROTOS_H
#define PORT_PROTOS_H

#include "fmgr.h"            /* for func_ptr */
#include "utils/dynamic_loader.h"

#ifdef LINUX_ELF

#include "dlfcn.h"

#endif

/* dynloader.c */

#ifndef LINUX_ELF
#define pg_dlsym(handle, funcname)	((func_ptr) dld_get_func((funcname)))
#define pg_dlclose(handle)		({ dld_unlink_by_file(handle, 1); free(handle); })
#else
/* #define	pg_dlopen(f)	dlopen(f, 1) */
#define    pg_dlopen(f)    dlopen(f, 2)
#define    pg_dlsym    dlsym
#define    pg_dlclose    dlclose
#define    pg_dlerror    dlerror
#endif

/* port.c */

#endif /* PORT_PROTOS_H */
