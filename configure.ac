dnl Autoconf configuration for rra-c-util.
dnl
dnl Written by Russ Allbery <rra@stanford.edu>
dnl Copyright 2000, 2003 Russ Allbery <rra@stanford.edu>
dnl Copyright 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009
dnl     Board of Trustees, Leland Stanford Jr. University
dnl
dnl See LICENSE for licensing terms.

dnl I use this introductory boilerplate for all configure.ac files, changing
dnl the AC_INIT arguments as appropriate.
AC_PREREQ([2.63])
AC_INIT([rra-c-util], [1.0], [rra@stanford.edu])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_LIBOBJ_DIR([portable])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.10])
AM_MAINTAINER_MODE

dnl Standard probes for the basic compilation environment.  These are
dnl generally the same for every package, except that ones that include shared
dnl libraries change AC_PROG_RANLIB to AC_PROG_LIBTOOL.
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Probes for networking libraries.  Only needed for packages that make or
dnl receive network connections or do DNS lookups.
AC_SEARCH_LIBS([gethostbyname], [nsl])
AC_SEARCH_LIBS([socket], [socket], ,
    [AC_CHECK_LIB([nsl], [socket], [LIBS="-lnsl -lsocket $LIBS"], ,
        [-lsocket])])

dnl Probe for Kerberos.  This is only needed for packages that use Kerberos,
dnl of course.
RRA_LIB_KRB5
RRA_LIB_KRB5_SWITCH
AC_CHECK_FUNCS([krb5_get_init_creds_opt_set_default_flags])
AC_CHECK_TYPES([krb5_realm], , , [#include <krb5.h>])
RRA_LIB_KRB5_RESTORE

dnl These probes are needed by anything that uses portable/system.h, the other
dnl portable replacements, and the messages utility library.  sys/filio.h is
dnl only used by fdflag.
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([sys/bittypes.h sys/filio.h sys/uio.h syslog.h])
AC_CHECK_DECLS([snprintf, vsnprintf])
RRA_C_C99_VAMACROS
RRA_C_GNU_VAMACROS
AC_CHECK_TYPES([long long])
AC_CHECK_TYPE([sig_atomic_t], ,
    [AC_DEFINE([sig_atomic_t], [int],
        [Define to int if <signal.h> does not define.])],
    [#include <sys/types.h>
     #include <signal.h>])
RRA_FUNC_SNPRINTF
AC_CHECK_FUNCS([setrlimit setsid])
AC_REPLACE_FUNCS([asprintf daemon getopt setenv strlcat strlcpy])

dnl Additional probes for networking portability, used for packages that have
dnl network code and support IPv6.
AC_CHECK_HEADERS([sys/select.h])
AC_CHECK_DECLS([inet_aton, inet_ntoa], , ,
    [#include <sys/types.h>
     #include <netinet/in.h>
     #include <arpa/inet.h>])
AC_CHECK_MEMBERS([struct sockaddr.sa_len], , ,
    [#include <sys/types.h>
     #include <sys/socket.h>])
AC_CHECK_TYPE([socklen_t], ,
    [AC_DEFINE([socklen_t], [int],
        [Define to int if <sys/socket.h> does not define.])],
    [#include <sys/types.h>
     #include <sys/socket.h>])
AC_CHECK_TYPES([struct sockaddr_in6],
    [AC_DEFINE([HAVE_INET6], [1],
        [Define to 1 if IPv6 library interfaces are available.])], ,
    [#include <sys/types.h>
     #include <netinet/in.h>])
AC_CHECK_TYPES([struct sockaddr_storage],
    [AC_CHECK_MEMBERS([struct sockaddr_storage.ss_family], , ,
        [#include <sys/types.h>
         #include <sys/socket.h>])], ,
    [#include <sys/types.h>
     #include <sys/socket.h>])
RRA_MACRO_IN6_ARE_ADDR_EQUAL
RRA_MACRO_SA_LEN
RRA_FUNC_INET_NTOA
AC_CHECK_FUNCS([getaddrinfo],
    [RRA_FUNC_GETADDRINFO_ADDRCONFIG],
    [AC_LIBOBJ([getaddrinfo])])
AC_REPLACE_FUNCS([getnameinfo inet_aton inet_ntop])

dnl Output section.  This is generally the same for all packages.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADER([config.h])
AC_OUTPUT
