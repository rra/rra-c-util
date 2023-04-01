# serial 1

dnl Find the compiler and linker flags for SQLite v3.
dnl
dnl Finds the compiler and linker flags for linking with the SQLite library.
dnl Provides the --with-sqlite3, --with-sqlite3-lib, and
dnl --with-sqlite3-include configure options to specify non-standard paths to
dnl the SQLite v3 libraries or header files.
dnl
dnl Provides the macro RRA_LIB_SQLITE3 and sets the substitution variables
dnl SQLITE3_CPPFLAGS, SQLITE3_LDFLAGS, and SQLITE3_LIBS.  Also provides
dnl RRA_LIB_SQLITE3_SWITCH to set CPPFLAGS, LDFLAGS, and LIBS to include the
dnl SQLite library, saving the current values first, and
dnl RRA_LIB_SQLITE3_RESTORE to restore those settings to before the last
dnl RRA_LIB_SQLITE3_SWITCH.  Defines HAVE_SQLITE3 and sets rra_use_SQLITE3 to
dnl true.
dnl
dnl Provides the RRA_LIB_SQLITE3_OPTIONAL macro, which should be used if
dnl SQLite support is optional.  This macro will still always set the
dnl substitution variables, but they'll be empty if the SQLite library is not
dnl found or if --without-sqlite3 is given.  Defines HAVE_SQLITE3 and sets
dnl rra_use_SQLITE3 to true if the SQLite library is found and
dnl --without-sqlite3 is not given.
dnl
dnl Depends on the lib-helper.m4 framework and the Autoconf macros that come
dnl with pkg-config.
dnl
dnl The canonical version of this file is maintained in the rra-c-util
dnl package, available at <https://www.eyrie.org/~eagle/software/rra-c-util/>.
dnl
dnl Written by Russ Allbery <eagle@eyrie.org>
dnl Copyright 2020, 2022 Russ Allbery <eagle@eyrie.org>
dnl Copyright 2014
dnl     The Board of Trustees of the Leland Stanford Junior University
dnl
dnl This file is free software; the authors give unlimited permission to copy
dnl and/or distribute it, with or without modifications, as long as this
dnl notice is preserved.
dnl
dnl SPDX-License-Identifier: FSFULLR

dnl Save the current CPPFLAGS, LDFLAGS, and LIBS settings and switch to
dnl versions that include the SQLite 3 flags.  Used as a wrapper, with
dnl RRA_LIB_SQLITE3_RESTORE, around tests.
AC_DEFUN([RRA_LIB_SQLITE3_SWITCH], [RRA_LIB_HELPER_SWITCH([SQLITE3])])

dnl Restore CPPFLAGS, LDFLAGS, and LIBS to their previous values before
dnl RRA_LIB_SQLITE3_SWITCH was called.
AC_DEFUN([RRA_LIB_SQLITE3_RESTORE], [RRA_LIB_HELPER_RESTORE([SQLITE3])])

dnl Checks if SQLite v3 is present.  The single argument, if "true", says to
dnl fail if the SQLite library could not be found.  Prefer probing with
dnl pkg-config if available and the --with flags were not given.
AC_DEFUN([_RRA_LIB_SQLITE3_INTERNAL],
[RRA_LIB_HELPER_PATHS([SQLITE3])
 AS_IF([test x"$SQLITE3_CPPFLAGS" = x && test x"$SQLITE3_LDFLAGS" = x],
    [PKG_CHECK_EXISTS([sqlite3],
        [PKG_CHECK_MODULES([SQLITE3], [sqlite3])
         SQLITE3_CPPFLAGS="$SQLITE3_CFLAGS"])])
 AS_IF([test x"$SQLITE3_LIBS" = x],
    [RRA_LIB_SQLITE3_SWITCH
     LIBS=
     AC_SEARCH_LIBS([sqlite3_open_v2], [sqlite3],
        [SQLITE3_LIBS="$LIBS"],
        [AS_IF([test x"$1" = xtrue],
            [AC_MSG_ERROR([cannot find usable libsqlite3 library])])])
     RRA_LIB_SQLITE3_RESTORE])
 RRA_LIB_SQLITE3_SWITCH
 AC_CHECK_HEADERS([sqlite3.h])
 RRA_LIB_SQLITE3_RESTORE])

dnl The main macro for packages with mandatory SQLite v3 support.
AC_DEFUN([RRA_LIB_SQLITE3],
[RRA_LIB_HELPER_VAR_INIT([SQLITE3])
 RRA_LIB_HELPER_WITH([sqlite3], [SQLite v3], [SQLITE3])
 _RRA_LIB_SQLITE3_INTERNAL([true])
 rra_use_SQLITE3=true
 AC_DEFINE([HAVE_SQLITE3], 1, [Define if libsqlite3 is available.])])

dnl The main macro for packages with optional SQLite v3 support.
AC_DEFUN([RRA_LIB_SQLITE3_OPTIONAL],
[RRA_LIB_HELPER_VAR_INIT([SQLITE3])
 RRA_LIB_HELPER_WITH_OPTIONAL([sqlite3], [SQLite v3], [SQLITE3])
 AS_IF([test x"$rra_use_SQLITE3" != xfalse],
    [AS_IF([test x"$rra_use_SQLITE3" = xtrue],
        [_RRA_LIB_SQLITE3_INTERNAL([true])],
        [_RRA_LIB_SQLITE3_INTERNAL([false])])])
 AS_IF([test x"$SQLITE3_LIBS" = x],
    [RRA_LIB_HELPER_VAR_CLEAR([SQLITE3])],
    [rra_use_SQLITE3=true
     AC_DEFINE([HAVE_SQLITE3], 1, [Define if libsqlite3 is available.])])])
