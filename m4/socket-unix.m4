dnl Various checks for UNIX domain socket support and macros.
dnl
dnl This is a collection of various Autoconf macros for checking UNIX domain
dnl socket properties.  The macros provided are:
dnl
dnl     RRA_MACRO_SUN_LEN
dnl     RRA_SYS_UNIX_SOCKETS
dnl
dnl They use a separate internal source macro to make the code easier to read.
dnl
dnl Copyright 2009 Board of Trustees, Leland Stanford Jr. University
dnl Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009
dnl     by Internet Systems Consortium, Inc. ("ISC")
dnl Copyright (c) 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
dnl     2002, 2003 by The Internet Software Consortium and Rich Salz
dnl
dnl See LICENSE for licensing terms.

dnl Source used by RRA_MACRO_SUN_LEN.
AC_DEFUN([_RRA_MACRO_SUN_LEN_SOURCE], [[
#include <sys/types.h>
#include <sys/un.h>

int
main(void)
{
    struct sockaddr_un s_un;
    int i = SUN_LEN(&s_un);
}
]])

dnl Check for SUN_LEN, which returns the size of a struct sockaddr_un.  Sets
dnl HAVE_SUN_LEN if the macro is available.
AC_DEFUN([RRA_MACRO_SUN_LEN],
[AC_CACHE_CHECK([for SUN_LEN macro], [rra_cv_sun_len_macro],
   [AC_LINK_IFELSE([AC_LANG_SOURCE([_RRA_MACRO_SUN_LEN_SOURCE])],
       [rra_cv_sun_len_macro=yes],
       [rra_cv_sun_len_macro=no])])
AS_IF([test x"$rra_cv_sun_len_macro" = xyes],
   [AC_DEFINE([HAVE_SUN_LEN], 1,
       [Define if <sys/un.h> defines the SUN_LEN macro.])])])

dnl Source used by RRA_SYS_UNIX_SOCKETS.
AC_DEFUN([_RRA_SYS_UNIX_SOCKETS], [[
#include <sys/socket.h>
#ifndef AF_UNIX
error: No UNIX domain sockets!
#endif
]])

dnl Check if UNIX domain sockets are supported.  Assume that they are if
dnl AF_UNIX is set in <sys/socket.h>.  This loses on really old versions of
dnl Linux, where AF_UNIX is available but doesn't work, but we don't care
dnl about Linux 1.0 any more.
AC_DEFUN([RRA_SYS_UNIX_SOCKETS],
[AC_CACHE_CHECK([for UNIX domain sockets], [rra_cv_sys_unix_sockets],
   [AC_COMPILE_IFELSE([AC_LANG_SOURCE([_RRA_SYS_UNIX_SOCKETS])],
       [rra_cv_sys_unix_sockets=yes],
       [rra_cv_sys_unix_sockets=no])])
AS_IF([test x"$rra_cv_sys_unix_sockets" = xyes],
   [AC_DEFINE([HAVE_UNIX_DOMAIN_SOCKETS], 1,
       [Define if you have UNIX domain sockets.])])])
