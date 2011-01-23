dnl Autoconf configuration for rra-c-util.
dnl
dnl Written by Russ Allbery <rra@stanford.edu>
dnl Copyright 2000, 2003 Russ Allbery <rra@stanford.edu>
dnl Copyright 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
dnl     The Board of Trustees of the Leland Stanford Junior University
dnl
dnl This file is free software; the authors give unlimited permission to copy
dnl and/or distribute it, with or without modifications, as long as this
dnl notice is preserved.

dnl I use this introductory boilerplate for all configure.ac files, changing
dnl the AC_INIT arguments as appropriate.
AC_PREREQ([2.64])
AC_INIT([rra-c-util], [3.0], [rra@stanford.edu])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_LIBOBJ_DIR([portable])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 check-news foreign silent-rules subdir-objects
    -Wall -Werror])
AM_MAINTAINER_MODE

dnl Standard probes for the basic compilation environment.  These are
dnl generally the same for every package, except that ones that include shared
dnl libraries change AC_PROG_RANLIB to AC_PROG_LIBTOOL.
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Probes for networking libraries.  Only needed for packages that make or
dnl receive network connections or do DNS lookups.
AC_SEARCH_LIBS([gethostbyname], [nsl])
AC_SEARCH_LIBS([socket], [socket], ,
    [AC_CHECK_LIB([nsl], [socket], [LIBS="-lnsl -lsocket $LIBS"], ,
        [-lsocket])])

dnl Probe for Kerberos.  This is only needed for packages that use Kerberos,
dnl of course.  These are the necessary probes for building the full
dnl portability layer and the kerberos.c TAP library add-on.  Many packages
dnl will need less than this.  This should possibly move into krb5.m4.
RRA_LIB_KRB5
RRA_LIB_KRB5_SWITCH
AC_CHECK_FUNCS([krb5_cc_copy_cache \
    krb5_data_free \
    krb5_free_default_realm \
    krb5_get_init_creds_opt_alloc \
    krb5_get_init_creds_opt_set_default_flags \
    krb5_init_secure_context \
    krb5_principal_get_comp_string \
    krb5_principal_get_num_comp \
    krb5_principal_get_realm \
    krb5_principal_set_realm \
    krb5_xfree])
AC_CHECK_FUNCS([krb5_get_init_creds_opt_free],
    [RRA_FUNC_KRB5_GET_INIT_CREDS_OPT_FREE_ARGS])
AC_CHECK_DECLS([krb5_kt_free_entry])
AC_CHECK_FUNCS([krb5_appdefault_string], ,
    [AC_CHECK_FUNCS([krb5_get_profile])
     AC_CHECK_HEADERS([k5profile.h profile.h])
     AC_LIBOBJ([krb5-profile])])
AC_CHECK_FUNCS([krb5_get_renewed_creds], ,
    [AC_CHECK_FUNCS([krb5_copy_creds_contents])
     AC_LIBOBJ([krb5-renew])])
RRA_LIB_KRB5_RESTORE

dnl Make sure we build with Kerberos support where it's optional.
AC_DEFINE([HAVE_KERBEROS], 1, [Define to enable Kerberos features.])

dnl Probe for GSS-API.  This is only needed for packages that use GSS-API, of
dnl course.  These are the probes needed for portable/gssapi.h and
dnl portable/gssapi-mech.c.  This should possibly move into gssapi.m4.
RRA_LIB_GSSAPI
RRA_LIB_GSSAPI_SWITCH
AC_CHECK_HEADERS([gssapi.h])
AC_CHECK_DECL([GSS_C_NT_USER_NAME],
    [AC_DEFINE([HAVE_GSS_RFC_OIDS], 1,
       [Define to 1 if the GSS-API library uses RFC-compliant OIDs.])], ,
[[
#if defined(HAVE_GSSAPI_H)
# include <gssapi.h>
#else
# include <gssapi/gssapi.h>
#endif
]])
AC_CHECK_DECLS([GSS_KRB5_MECHANISM], ,
    [AC_CHECK_DECLS([gss_mech_krb5], ,
        [AC_LIBOBJ([gssapi-mech])],
        [#include <gssapi/gssapi_krb5.h>])],
[[
#if defined(HAVE_GSSAPI_H)
# include <gssapi.h>
#else
# include <gssapi/gssapi.h>
#endif
]])
RRA_LIB_GSSAPI_RESTORE

dnl Probe for Kerberos v4.  This is only needed for packages that use Kerberos
dnl v4, of course.  This code includes probing for the lifetime support
dnl functions, for which we have a replacement in portable/lifetime.c.
RRA_LIB_KRB4_OPTIONAL
AS_IF([test x"$rra_use_krb4" = xyes],
    [RRA_LIB_KRB4_SWITCH
     AC_CHECK_FUNCS([krb_life_to_time], , [AC_LIBOBJ([lifetime])])
     RRA_LIB_KRB4_RESTORE])

dnl Probe for libkafs.  Do this so that we can test the libkafs portability
dnl library.  If AFS support should be conditional, set rra_build_kafs
dnl explicitly to false, conditionally run RRA_LIB_KAFS, and then always set
dnl the Automake conditional.
RRA_LIB_KAFS
AM_CONDITIONAL([NEED_KAFS], [test x"$rra_build_kafs" = xtrue])

dnl Probe for PAM.  These are the tests required for portable/pam.h and its
dnl replacement functions.
AC_SEARCH_LIBS([pam_set_data], [pam])
AC_CHECK_FUNCS([pam_getenv pam_getenvlist pam_modutil_getpwnam])
AC_REPLACE_FUNCS([pam_syslog pam_vsyslog])
AC_CHECK_HEADERS([security/pam_modutil.h], ,
    [AC_CHECK_HEADERS([pam/pam_modutil.h])])
AC_CHECK_HEADERS([security/pam_appl.h], ,
    [AC_CHECK_HEADERS([pam/pam_appl.h], ,
        [AC_MSG_ERROR([No PAM header files found])])])
AC_CHECK_HEADERS([security/pam_ext.h], ,
    [AC_CHECK_HEADERS([pam/pam_ext.h])])
RRA_HEADER_PAM_CONST
AC_DEFINE([MODULE_NAME], ["pam_example"],
    [The name of the PAM module, used by the pam_vsyslog replacement.])

dnl These probes are needed by anything that uses portable/system.h, the other
dnl portable replacements, and the messages utility library.  sys/filio.h is
dnl only used by fdflag.
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([sys/bittypes.h sys/filio.h sys/uio.h syslog.h])
AC_CHECK_DECLS([snprintf, vsnprintf])
RRA_C_C99_VAMACROS
RRA_C_GNU_VAMACROS
AC_TYPE_LONG_LONG_INT
AC_TYPE_UINT32_T
AC_CHECK_TYPE([sig_atomic_t], ,
    [AC_DEFINE([sig_atomic_t], [int],
        [Define to int if <signal.h> does not define.])],
    [#include <sys/types.h>
     #include <signal.h>])
RRA_FUNC_SNPRINTF
AC_CHECK_FUNCS([setrlimit setsid])
AC_REPLACE_FUNCS([asprintf daemon getopt issetuidgid mkstemp setenv seteuid])
AC_REPLACE_FUNCS([strlcat strlcpy strndup])

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

dnl Additional probes for UNIX domain socket support.  These are only needed
dnl by packages that want to use UNIX domain sockets.
RRA_SYS_UNIX_SOCKETS
RRA_MACRO_SUN_LEN

dnl Output section.  This is generally the same for all packages.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADER([config.h])
AC_OUTPUT
