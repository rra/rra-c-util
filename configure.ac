dnl Autoconf configuration for rra-c-util.
dnl
dnl Written by Russ Allbery <rra@stanford.edu>
dnl Copyright 2000, 2003 Russ Allbery <rra@stanford.edu>
dnl Copyright 2002, 2003, 2004, 2005, 2006, 2007, 2008
dnl     Board of Trustees, Leland Stanford Jr. University
dnl
dnl See LICENSE for licensing terms.

AC_PREREQ([2.61])
AC_INIT([rra-c-util], [1.0], [rra@stanford.edu])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_LIBOBJ_DIR([portable])
AM_INIT_AUTOMAKE([1.10])
AM_MAINTAINER_MODE

AC_PROG_CC
AC_AIX
AC_GNU_SOURCE
AC_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_SEARCH_LIBS([gethostbyname], [nsl])
AC_SEARCH_LIBS([socket], [socket], ,
    [AC_CHECK_LIB([nsl], [socket], [LIBS="-lnsl -lsocket $LIBS"], ,
        [-lsocket])])
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([sys/bittypes.h sys/filio.h sys/select.h sys/uio.h syslog.h])
AC_CHECK_DECLS([snprintf, vsnprintf])
AC_CHECK_DECLS([inet_aton, inet_ntoa], , ,
    [#include <sys/types.h>
     #include <netinet/in.h>
     #include <arpa/inet.h>])
RRA_C_C99_VAMACROS
RRA_C_GNU_VAMACROS
AC_CHECK_MEMBERS([struct sockaddr.sa_len], , ,
    [#include <sys/types.h>
     #include <sys/socket.h>])
AC_CHECK_TYPES([long long])
AC_CHECK_TYPE([sig_atomic_t], ,
    [AC_DEFINE([sig_atomic_t], [int],
        [Define to int if <signal.h> does not define.])],
    [#include <sys/types.h>
     #include <signal.h>])
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
RRA_FUNC_SNPRINTF
AC_CHECK_FUNCS([setsid])
AC_REPLACE_FUNCS([inet_aton setenv])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADER([config.h])
AC_OUTPUT
