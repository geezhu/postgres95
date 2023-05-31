# src/backend/catalog/CMakeLists.txt
option(ALLOW_PG_GROUP "ALLOW_PG_GROUP" OFF)
set(BKIOPTS "")
if ("${ALLOW_PG_GROUP}" STREQUAL "ON")
    set(BKIOPTS -DALLOW_PG_GROUP)
endif ()

option(TIOGA "TIOGA" OFF)
set(MACRO_OPTIONS "")
##############################################################################
#
# CONFIGURATION SECTION
#
# Following are settings pertaining to the postgres build and 
# installation.  The most important one is obviously the name 
# of the port.

#  The name of the port.  Valid choices are:
#	alpha		-	DEC Alpha AXP on OSF/1 2.0
#	hpux		-	HP PA-RISC on HP-UX 9.0
#	sparc_solaris	-	SUN SPARC on Solaris 2.4
#	sparc		-	SUN SPARC on SunOS 4.1.3
#	ultrix4		-	DEC MIPS on Ultrix 4.4
#	linux		-	Intel x86 on Linux 1.2 and Linux ELF
#				(For non-ELF Linux, you need to comment out 
#				"LINUX_ELF=1" in src/mk/port/postgres.mk.linux)
#	BSD44_derived	-	OSs derived from 4.4-lite BSD (NetBSD, FreeBSD)
#       bsdi            -       BSD/OS 2.0 and 2.01
#	aix		-	IBM on AIX 3.2.5
#	irix5		-	SGI MIPS on IRIX 5.3
#  Some hooks are provided for
#	svr4		-	Intel x86 on Intel SVR4
#	next		-	Motorola MC68K or Intel x86 on NeXTSTEP 3.2
#  but these are guaranteed not to work as of yet.
#
#  XXX	Note that you MUST set PORTNAME here (or on the command line) so 
#	that port-dependent variables are correctly set within this file.
#	Makefile.custom does not take effect (for ifeq purposes) 
#	until after this file is processed!
#  make sure that you have no whitespaces after the PORTNAME setting
#  or the makefiles can get confused
set(PORTNAME linux)

# POSTGRESLOGIN is the login name of the user who gets special
# privileges within the database.  By default it is "postgres", but
# you can change it to any existing login name (such as your own 
# login if you are compiling a private version or don't have root
# access).
set(POSTGRESLOGIN postgres)

# For convenience, POSTGRESDIR is where DATADIR, BINDIR, and LIBDIR 
# and other target destinations are rooted.  Of course, each of these is 
# changable separately.
set(POSTGRESDIR /private/postgres95)

# SRCDIR specifies where the source files are.
set(SRCDIR ${POSTGRESDIR}/src)

# DATADIR specifies where the postmaster expects to find its database.
# This may be overridden by command line options or the PGDATA environment
# variable.
set(DATADIR ${POSTGRESDIR}/data)

# Where the postgres executables live (changeable by just putting them
# somewhere else and putting that directory in your shell PATH)
set(BINDIR ${POSTGRESDIR}/bin)

# Where libpq.a gets installed.  You must put it where your loader will
# look for it if you wish to use the -lpq convention.  Otherwise you
# can just put the absolute pathname to the library at the end of your
# command line.
set(LIBDIR ${POSTGRESDIR}/lib)

# This is the directory where IPC utilities ipcs and ipcrm are located
#
set(IPCSDIR /usr/bin)

# Where the man pages (suitable for use with "man") get installed.
set(POSTMANDIR	${POSTGRESDIR}/man)

# Where the formatted documents (e.g., the reference manual) get installed.
set(POSTDOCDIR	${POSTGRESDIR}/doc)

# Where the header files necessary to build frontend programs get installed.
set(HEADERDIR	${POSTGRESDIR}/include)

# NAMEDATALEN is the max length for system identifiers (e.g. table names, 
# attribute names, function names, etc.)  
#
# These MUST be set here.  DO NOT COMMENT THESE OUT
# Setting these too high will result in excess space usage for system catalogs
# Setting them too low will make the system unusable.
# values between 16 and 64 that are multiples of four are recommended.
#
# NOTE also that databases with different NAMEDATALEN's cannot interoperate!
#
set(NAMEDATALEN	32)

# OIDNAMELEN should be set to NAMEDATALEN + sizeof(Oid)
set(OIDNAMELEN	36)
#list(APPEND CFLAGS -DNAMEDATALEN=${NAMEDATALEN} -DOIDNAMELEN=${OIDNAMELEN})
list(APPEND MACRO_OPTIONS -DNAMEDATALEN=${NAMEDATALEN} -DOIDNAMELEN=${OIDNAMELEN} -DPORTNAME_${PORTNAME})
##############################################################################
#
# FEATURES 
#
# To disable a feature, comment out the entire definition
# (that is, prepend '#', don't set it to "0" or "no").

# Comment out ENFORCE_ALIGNMENT if you do NOT want unaligned access to
# multi-byte types to generate a bus error.
set(ENFORCE_ALIGNMENT true)

# Comment out CDEBUG to turn off debugging and sanity-checking.
#
#	XXX on MIPS, use -g3 if you want to compile with -O
set(CDEBUG -g)

# turn this on if you prefer European style dates instead of American
# style dates
# EUROPEAN_DATES = 1

# Comment out PROFILE to disable profiling.
#
#	XXX define on MIPS if you want to be able to use pixie.
#	    note that this disables dynamic loading!
#PROFILE= -p -non_shared

# About the use of readline in psql:
#    psql does not require the GNU readline and history libraries. Hence, we
#    do not compile with them by default. However, there are hooks in the
#    program which supports the use of GNU readline and history. Should you
#    decide to use them, change USE_READLINE to true and change READLINE_INCDIR
#    and READLINE_LIBDIR to reflect the location of the readline and histroy
#    headers and libraries.
#
#USE_READLINE= true

# directories for the readline and history libraries.
set(READLINE_INCDIR /usr/include/)
set(HISTORY_INCDIR /usr/include/)
set(READLINE_LIBDIR /usr/lib/x86_64-linux-gnu/)
set(HISTORY_LIBDIR /usr/lib/x86_64-linux-gnu/)

# If you do not plan to use Host based authentication,
# comment out the following line
#HBA = 1
set(HBA 1)
set(HBAFLAGS "")

if (DEFINED HBA)
    set(HBAFLAGS -DHBA)
endif ()



# If you plan to use Kerberos for authentication...
#
# Comment out KRBVERS if you do not use Kerberos.
# 	Set KRBVERS to "4" for Kerberos v4, "5" for Kerberos v5.
#	XXX Edit the default Kerberos variables below!
#
#set(KRBVERS 5)

# Globally pass Kerberos file locations.
#	these are used in the postmaster and all libpq applications.
#
#	Adjust KRBINCS and KRBLIBS to reflect where you have Kerberos
#		include files and libraries installed.
#	PG_KRB_SRVNAM is the name under which POSTGRES is registered in
#		the Kerberos database (KDC).
#	PG_KRB_SRVTAB is the location of the server's keytab file.
#
set(KRBINCS "")
set(KRBLIBS "")
set(KRBFLAGS "")
if (DEFINED KRBVERS)
    set(KRBINCS -I/usr/athena/include)
    set(KRBLIBS -L/usr/athena/lib)
    set(KRBFLAGS ${KRBINCS} -DPG_KRB_SRVNAM='"postgres_dbms"')
    if ("${KRBVERS}" STREQUAL "4")
        list(APPEND KRBFLAGS -DKRB4)
        list(APPEND KRBFLAGS -DPG_KRB_SRVTAB='"/etc/srvtab"')
        list(APPEND KRBLIBS -lkrb -ldes)
    elseif ("${KRBVERS}" STREQUAL "5")
        list(APPEND KRBFLAGS -DKRB5)
        list(APPEND KRBFLAGS -DPG_KRB_SRVTAB='"FILE:/krb5/srvtab.postgres"')
        list(APPEND KRBLIBS -lkrb5 -lcrypto -lcom_err -lisode)
    endif ()
endif ()

#
# location of Tcl/Tk headers and libraries
#
# Uncomment this to build the tcl utilities.

set(USE_TCL true)
# customize these to your site's needs
#
set(TCL_INCDIR /usr/include/tcl8.6/)
set(TCL_LIBDIR /usr/lib/x86_64-linux-gnu/)
set(TCL_LIB -ltcl8.6)
set(TK_INCDIR /usr/include/tcl8.6/)
set(TK_LIBDIR /usr/lib/x86_64-linux-gnu/)
set(TK_LIB -ltk8.6)

#
# include port specific rules and variables. For instance:
#
# signal(2) handling - this is here because it affects some of 
# the frontend commands as well as the backend server.
#
# Ultrix and SunOS provide BSD signal(2) semantics by default.
#
# SVID2 and POSIX signal(2) semantics differ from BSD signal(2) 
# semantics.  We can use the POSIX sigaction(2) on systems that
# allow us to request restartable signals (SA_RESTART).
#
# Some systems don't allow restartable signals at all unless we 
# link to a special BSD library.
#
# We devoutly hope that there aren't any systems that provide
# neither POSIX signals nor BSD signals.  The alternative 
# is to do signal-handler reinstallation, which doesn't work well 
# at all.
#
#-include $(MKDIR)/port/postgres.mk.$(PORTNAME)

##############################################################################
#
# Flags for CC and LD. (depend on CDEBUG and PROFILE)
#

# Globally pass debugging/optimization/profiling flags based
# on the options selected above.
set(CFLAGS_OPT_TMP ${CFLAGS_OPT})
if (DEFINED CDEBUG)
    list(APPEND CFLAGS ${CDEBUG})
    list(APPEND LDFLAGS ${CDEBUG})
else()
    if (NOT DEFINED CFLAGS_OPT)
        set(CFLAGS_OPT_TMP -O)
    endif ()
    list(APPEND ${CFLAGS_OPT_TMP})
#
# Uncommenting this will make things go a LOT faster, but you will
# also lose a lot of useful error-checking.
#
    list(APPEND CFLAGS -DNO_ASSERT_CHECKING)
    list(APPEND MACRO_OPTIONS -DNO_ASSERT_CHECKING)
endif ()

if (DEFINED PROFILE)
    list(APPEND CFLAGS ${PROFILE})
    list(APPEND LDFLAGS ${PROFILE})
endif ()

# Globally pass PORTNAME
list(APPEND CFLAGS -DPORTNAME_${PORTNAME})

# Globally pass the default TCP port for postmaster(1).
list(APPEND MACRO_OPTIONS -DPOSTPORT="5432")
# include flags from mk/port/postgres.mk.$(PORTNAME)
list(APPEND CFLAGS ${CFLAGS_BE})
list(APPEND LDADD ${LDADD_BE})
list(APPEND LDFLAGS ${LDFLAGS_BE})


##############################################################################
#
# Miscellaneous configuration
#

# This is the time, in seconds, at which a given backend server
# will wait on a lock before deciding to abort the transaction
# (this is what we do in lieu of deadlock detection).
#
# Low numbers are not recommended as they will tend to cause
# false aborts if many transactions are long-lived.
list(APPEND CFLAGS -DDEADLOCK_TIMEOUT=60)
list(APPEND MACRO_OPTIONS -DDEADLOCK_TIMEOUT=60)