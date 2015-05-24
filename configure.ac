dnl Autoconf configuration for rra-c-util.
dnl
dnl Written by Russ Allbery <eagle@eyrie.org>
dnl Copyright 2000, 2003, 2013, 2014, 2015 Russ Allbery <eagle@eyrie.org>
dnl Copyright 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011,
dnl     2012, 2013, 2014
dnl     The Board of Trustees of the Leland Stanford Junior University
dnl
dnl This file is free software; the authors give unlimited permission to copy
dnl and/or distribute it, with or without modifications, as long as this
dnl notice is preserved.

dnl I use this introductory boilerplate for all configure.ac files, changing
dnl the AC_INIT arguments as appropriate.
AC_PREREQ([2.64])
AC_INIT([rra-c-util], [5.7], [eagle@eyrie.org])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_LIBOBJ_DIR([portable])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 check-news dist-xz foreign silent-rules subdir-objects
    -Wall -Werror])
AM_MAINTAINER_MODE

dnl Detect unexpanded macros.
m4_pattern_forbid([^_?RRA_])

dnl Standard probes for the basic compilation environment.  These are
dnl generally the same for every package, except that ones that include shared
dnl libraries change AC_PROG_RANLIB to AC_PROG_LIBTOOL.  Add
dnl RRA_LD_VERSION_SCRIPT to probe for symbol versioning support for packages
dnl using shared libraries.
dnl
dnl AM_PROG_AR is required for Automake 1.12 by Libtool but not defined at all
dnl (or needed) in Automake 1.11.  Work around this bug.
AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
RRA_PROG_CC_CLANG
AC_SYS_LARGEFILE
AM_PROG_CC_C_O
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Probes for networking libraries.  Only needed for packages that make or
dnl receive network connections or do DNS lookups.
AC_SEARCH_LIBS([gethostbyname], [nsl])
AC_SEARCH_LIBS([socket], [socket], ,
    [AC_CHECK_LIB([nsl], [socket], [LIBS="-lnsl -lsocket $LIBS"], [],
        [-lsocket])])

dnl Probe for Kerberos.  This is only needed for packages that use Kerberos,
dnl of course.  These are the necessary probes for building the full
dnl portability layer and the kerberos.c TAP library add-on.  Many packages
dnl will need less than this.  This should possibly move into krb5.m4.
RRA_LIB_KRB5
RRA_LIB_KRB5_SWITCH
AC_CHECK_TYPES([krb5_realm], [], [], [RRA_INCLUDES_KRB5])
AC_CHECK_FUNCS([krb5_cc_copy_cache \
    krb5_cc_get_full_name \
    krb5_data_free \
    krb5_free_default_realm \
    krb5_free_string \
    krb5_get_init_creds_opt_alloc \
    krb5_get_init_creds_opt_set_change_password_prompt \
    krb5_get_init_creds_opt_set_default_flags \
    krb5_init_secure_context \
    krb5_principal_get_comp_string \
    krb5_principal_get_num_comp \
    krb5_principal_get_realm \
    krb5_principal_set_realm \
    krb5_set_password \
    krb5_verify_init_creds_opt_init \
    krb5_xfree])
AC_CHECK_FUNCS([krb5_get_init_creds_opt_set_pkinit],
    [RRA_FUNC_KRB5_GET_INIT_CREDS_OPT_SET_PKINIT_ARGS])
AC_CHECK_FUNCS([krb5_get_init_creds_opt_free],
    [RRA_FUNC_KRB5_GET_INIT_CREDS_OPT_FREE_ARGS])
AC_CHECK_DECLS([krb5_kt_free_entry], [], [], [RRA_INCLUDES_KRB5])
AC_CHECK_FUNCS([krb5_appdefault_string], [],
    [AC_CHECK_FUNCS([krb5_get_profile])
     AC_CHECK_HEADERS([k5profile.h profile.h])
     AC_LIBOBJ([krb5-profile])])
AC_CHECK_FUNCS([krb5_get_renewed_creds], [],
    [AC_CHECK_FUNCS([krb5_copy_creds_contents])
     AC_LIBOBJ([krb5-renew])])
AC_LIBOBJ([krb5-extra])
RRA_LIB_KRB5_RESTORE

dnl Probe for GSS-API.  This is only needed for packages that use GSS-API.
dnl The mechanism probe is not included in RRA_LIB_GSSAPI since it requires
dnl a replacement source file and is only needed for packages that need to
dnl refer to the Kerberos mechanism OID.
RRA_LIB_GSSAPI
RRA_LIB_GSSAPI_SWITCH
AC_CHECK_DECLS([GSS_KRB5_MECHANISM], [],
   [AC_CHECK_DECLS([gss_mech_krb5], [],
       [AC_LIBOBJ([gssapi-mech])], [RRA_INCLUDES_GSSAPI])],
   [RRA_INCLUDES_GSSAPI])
RRA_LIB_GSSAPI_RESTORE

dnl Probe for Kerberos kadmin client libraries.
RRA_LIB_KADM5CLNT_OPTIONAL
RRA_LIB_KADM5CLNT_SWITCH
AC_CHECK_HEADERS([kadm5/kadm5_err.h])
AC_CHECK_FUNCS([kadm5_init_with_skey_ctx])
RRA_LIB_KADM5CLNT_RESTORE

dnl Probe for libevent.
dnl Check for libevent, used by the server.
RRA_LIB_EVENT_OPTIONAL
AS_IF([test x"$rra_use_LIBEVENT" = xtrue],
   [RRA_LIB_EVENT_SWITCH
    AC_CHECK_HEADERS([event2/event.h])
    AC_CHECK_TYPES([bufferevent_data_cb, bufferevent_event_cb,
                    event_callback_fn, evutil_socket_t],
        [], [], [RRA_INCLUDES_EVENT])
    AC_CHECK_FUNCS([bufferevent_get_input \
        bufferevent_read_buffer \
        bufferevent_socket_new \
        evbuffer_get_length \
        event_base_got_break \
        event_base_loopbreak \
        event_free \
        event_new \
        event_set_fatal_callback \
        libevent_global_shutdown])
    AC_LIBOBJ([event-extra])
    RRA_LIB_EVENT_RESTORE])

dnl Probe for libkafs.  Do this so that we can test the libkafs portability
dnl library.  For rra-c-util, AFS support is conditional and not built by
dnl default, so this is an example of that.  For packages where it is not
dnl conditional, remove the test around RRA_LIB_KAFS and run it
dnl unconditionally.
rra_build_kafs=false
AC_ARG_ENABLE([kafs],
    [AC_HELP_STRING([--enable-kafs], [Enable kafs library support])],
    [AS_IF([test x"$enableval" != xno], [RRA_LIB_KAFS])])
AM_CONDITIONAL([NEED_KAFS], [test x"$rra_build_kafs" = xtrue])

dnl Probe for PAM.  These are the tests required for portable/pam.h and its
dnl replacement functions.
AC_SEARCH_LIBS([pam_set_data], [pam])
AC_CHECK_FUNCS([pam_getenv pam_getenvlist pam_modutil_getpwnam])
AC_REPLACE_FUNCS([pam_syslog pam_vsyslog])
AC_CHECK_HEADERS([security/pam_modutil.h], [],
    [AC_CHECK_HEADERS([pam/pam_modutil.h])])
AC_CHECK_HEADERS([security/pam_appl.h], [],
    [AC_CHECK_HEADERS([pam/pam_appl.h], [],
        [AC_MSG_ERROR([No PAM header files found])])])
AC_CHECK_HEADERS([security/pam_ext.h], [],
    [AC_CHECK_HEADERS([pam/pam_ext.h])])
RRA_HEADER_PAM_CONST
RRA_HEADER_PAM_STRERROR_CONST
AC_DEFINE([MODULE_NAME], ["pam_example"],
    [The name of the PAM module, used by the pam_vsyslog replacement.])

dnl Only required for the PAM test script facility.
AC_CHECK_HEADER([regex.h], [AC_CHECK_FUNCS([regcomp])])

dnl Run other optional library probing checks, even though they're unused by
dnl the rra-c-util code itself, so that they're exercised during the
dnl rra-c-util configure run.
RRA_LIB_BDB_OPTIONAL
AS_IF([test x"$rra_use_BDB" = xtrue], [RRA_LIB_BDB_NDBM])
RRA_LIB_CDB_OPTIONAL
RRA_LIB_OPENSSL_OPTIONAL
RRA_LIB_PCRE_OPTIONAL
RRA_LIB_REMCTL_OPTIONAL
RRA_LIB_SASL_OPTIONAL
RRA_LIB_SQLITE_OPTIONAL
RRA_LIB_SYSTEMD_DAEMON_OPTIONAL
RRA_LIB_ZLIB_OPTIONAL

dnl These probes are needed by anything that uses portable/system.h, the other
dnl portable replacements, and the messages utility library.  sys/filio.h is
dnl only used by fdflag.
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([sys/bitypes.h sys/filio.h sys/uio.h sys/time.h syslog.h])
AC_CHECK_DECLS([snprintf, strlcat, strlcpy, vsnprintf])
RRA_C_C99_VAMACROS
RRA_C_GNU_VAMACROS
AC_TYPE_LONG_LONG_INT
AC_TYPE_UINT32_T
AC_CHECK_TYPES([sig_atomic_t], [], [],
    [#include <sys/types.h>
     #include <signal.h>])
AC_CHECK_TYPES([ssize_t], [], [],
    [#include <sys/types.h>])
RRA_FUNC_SNPRINTF
AC_CHECK_FUNCS([setrlimit setsid])
AC_REPLACE_FUNCS([asprintf daemon getopt issetugid mkstemp reallocarray])
AC_REPLACE_FUNCS([setenv seteuid strlcat strlcpy strndup])

dnl Additional probes for networking portability, used for packages that have
dnl network code and support IPv6.  Probing for sys/select.h is also required
dnl for any package that uses the process TAP add-on.
AC_CHECK_HEADERS([sys/select.h])
AC_CHECK_DECLS([h_errno], [], [], [#include <netdb.h>])
AC_CHECK_DECLS([inet_aton, inet_ntoa], [], [],
    [#include <sys/types.h>
     #include <netinet/in.h>
     #include <arpa/inet.h>])
AC_CHECK_MEMBERS([struct sockaddr.sa_len], [], [],
    [#include <sys/types.h>
     #include <sys/socket.h>])
AC_CHECK_TYPES([socklen_t], [], [],
    [#include <sys/types.h>
     #include <sys/socket.h>])
AC_CHECK_TYPES([struct sockaddr_in6],
    [AC_DEFINE([HAVE_INET6], [1],
        [Define to 1 if IPv6 library interfaces are available.])], [],
    [#include <sys/types.h>
     #include <netinet/in.h>])
AC_CHECK_TYPES([struct sockaddr_storage],
    [AC_CHECK_MEMBERS([struct sockaddr_storage.ss_family], [], [],
        [#include <sys/types.h>
         #include <sys/socket.h>])], [],
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

dnl Enable appropriate warnings.
AM_CONDITIONAL([WARNINGS_GCC], [test x"$GCC" = xyes && test x"$CLANG" != xyes])
AM_CONDITIONAL([WARNINGS_CLANG], [test x"$CLANG" = xyes])

dnl Output section.  This is generally the same for all packages.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADER([config.h])
AC_OUTPUT
