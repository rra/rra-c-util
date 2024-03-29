# serial 1

dnl Probe for systemd libraries and installation paths.
dnl
dnl Provides the RRA_WITH_SYSTEMD_UNITDIR macro, which adds the
dnl --with-systemdsystemunitdir configure flag, sets the systemdsystemunitdir
dnl substitution variable, and provides the HAVE_SYSTEMD Automake conditional
dnl to use to control whether to install unit files.
dnl
dnl Provides the RRA_LIB_SYSTEMD_DAEMON_OPTIONAL macro, which sets
dnl SYSTEMD_CFLAGS and SYSTEMD_LIBS substitution variables if libsystemd is
dnl available and defines HAVE_SD_NOTIFY.  Adds sd-daemon.c to LIBOBJS if
dnl libsystemd is not available.  pkg-config support for libsystemd-daemon is
dnl required for it to be detected.
dnl
dnl Depends on the Autoconf macros that come with pkg-config.
dnl
dnl The canonical version of this file is maintained in the rra-c-util
dnl package, available at <https://www.eyrie.org/~eagle/software/rra-c-util/>.
dnl
dnl Written by Russ Allbery <eagle@eyrie.org>
dnl Copyright 2015, 2021, 2022 Russ Allbery <eagle@eyrie.org>
dnl Copyright 2013-2014
dnl     The Board of Trustees of the Leland Stanford Junior University
dnl
dnl This file is free software; the authors give unlimited permission to copy
dnl and/or distribute it, with or without modifications, as long as this
dnl notice is preserved.
dnl
dnl SPDX-License-Identifier: FSFULLR

dnl Determine the systemd system unit directory, along with a configure flag
dnl to override, and sets @systemdsystemunitdir@.  Provides the Automake
dnl HAVE_SYSTEMD Automake conditional.
AC_DEFUN([RRA_WITH_SYSTEMD_UNITDIR],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])
 AS_IF([test x"$PKG_CONFIG" = x], [PKG_CONFIG=false])
 AC_ARG_WITH([systemdsystemunitdir],
    [AS_HELP_STRING([--with-systemdsystemunitdir=DIR],
        [Directory for systemd service files])],
    [],
    [with_systemdsystemunitdir=`$PKG_CONFIG --variable=systemdsystemunitdir systemd`])
 AS_IF([test x"$with_systemdsystemunitdir" != xno],
    [AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])])
 AM_CONDITIONAL([HAVE_SYSTEMD],
    [test -n "$with_systemdsystemunitdir" \
        && test x"$with_systemdsystemunitdir" != xno])])

dnl Check for libsystemd or libsystemd-daemon and define SYSTEMD_{CFLAGS,LIBS}
dnl if it is available.  This is called RRA_LIB_SYSTEMD_DAEMON_OPTIONAL since
dnl it was originally written when libsystemd-daemon was separate, and only
dnl checks for that library.  It may eventually make sense to retire this in
dnl favor of a simple RRA_LIB_SYSTEMD_OPTIONAL that isn't backward-compatible.
AC_DEFUN([RRA_LIB_SYSTEMD_DAEMON_OPTIONAL],
[PKG_CHECK_EXISTS([libsystemd],
    [PKG_CHECK_MODULES([SYSTEMD], [libsystemd])
     AC_DEFINE([HAVE_SD_NOTIFY], 1, [Define if sd_notify is available.])],
    [PKG_CHECK_EXISTS([libsystemd-daemon],
        [PKG_CHECK_MODULES([SYSTEMD], [libsystemd-daemon])
         AC_DEFINE([HAVE_SD_NOTIFY], 1,
            [Define if sd_notify is available.])],
        [AC_LIBOBJ([sd-daemon])])])])
