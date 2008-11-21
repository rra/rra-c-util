# Automake makefile for rra-c-util.
#
# Written by Russ Allbery <rra@stanford.edu
# Copyright 2008 Board of Trustees, Leland Stanford Jr. University
#
# See LICENSE for licensing terms.

# The list of files needed by the test suite.  They're listed here to be added
# to EXTRA_DIST and so that they can be copied over properly for builddir !=
# srcdir builds.
TEST_FILES = portable/winsock.c tests/util/xmalloc-t

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = LICENSE autogen $(TEST_FILES)

CPPFLAGS = -I$(srcdir)/include

noinst_LIBRARIES = portable/libportable.a util/libutil.a
portable_libportable_a_SOURCES = portable/dummy.c portable/getaddrinfo.h \
	portable/getnameinfo.h portable/getopt.h portable/macros.h \
	portable/socket.h portable/stdbool.h portable/system.h \
	portable/uio.h
portable_libportable_a_LIBADD = $(LIBOBJS)
util_libutil_a_SOURCES = util/concat.c util/fdflag.c util/messages.c \
	util/network.c util/vector.c util/xmalloc.c util/xwrite.c

# Work around the GNU Coding Standards, which leave all the Autoconf and
# Automake stuff around after make maintainer-clean, thus making that command
# mostly worthless.
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 build-aux/depcomp \
        build-aux/install-sh build-aux/missing config.h.in config.h.in~ \
	configure

# Remove the test files we copied over if srcdir != builddir.
distclean-local:
	set -e; if [ x"$(builddir)" != x"$(srcdir)" ] ; then \
	    for f in $(TEST_FILES) ; do \
		rm -f "$(builddir)/$$f" ; \
	    done ; \
	fi

# A set of flags for warnings.  Add -O because gcc won't find some warnings
# without optimization turned on, and add -DDEBUG=1 so we'll also compile all
# debugging code and test it.
WARNINGS = -g -O -DDEBUG=1 -Wall -W -Wendif-labels -Wpointer-arith \
	-Wbad-function-cast -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wnested-externs -Werror

warnings:
	$(MAKE) CFLAGS='$(WARNINGS)'
	$(MAKE) CFLAGS='$(WARNINGS)' $(check_PROGRAMS)

# The bits below are for the test suite, not for the main package.
check_PROGRAMS = tests/runtests tests/portable/asprintf-t \
	tests/portable/daemon-t tests/portable/getaddrinfo-t \
	tests/portable/getnameinfo-t tests/portable/getopt-t \
	tests/portable/inet_aton-t tests/portable/inet_ntoa-t \
	tests/portable/inet_ntop-t tests/portable/setenv-t \
	tests/portable/snprintf-t tests/portable/strlcat-t \
	tests/portable/strlcpy-t tests/util/concat-t tests/util/fdflag-t \
	tests/util/messages-t tests/util/network-t tests/util/vector-t \
	tests/util/xmalloc tests/util/xwrite-t
check_LIBRARIES = tests/libtest.a
tests_libtest_a_SOURCES = tests/libtest.c tests/libtest.h

# All of the test programs.
tests_portable_asprintf_t_SOURCES = tests/portable/asprintf-t.c \
	tests/portable/asprintf.c
tests_portable_asprintf_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a 
tests_portable_daemon_t_SOURCES = tests/portable/daemon-t.c \
	tests/portable/daemon.c
tests_portable_daemon_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_getaddrinfo_t_SOURCES = tests/portable/getaddrinfo-t.c \
	tests/portable/getaddrinfo.c
tests_portable_getaddrinfo_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a 
tests_portable_getnameinfo_t_SOURCES = tests/portable/getnameinfo-t.c \
	tests/portable/getnameinfo.c
tests_portable_getnameinfo_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_getopt_t_SOURCES = tests/portable/getopt-t.c \
	tests/portable/getopt.c
tests_portable_getopt_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_inet_aton_t_SOURCES = tests/portable/inet_aton-t.c \
	tests/portable/inet_aton.c
tests_portable_inet_aton_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_inet_ntoa_t_SOURCES = tests/portable/inet_ntoa-t.c \
	tests/portable/inet_ntoa.c
tests_portable_inet_ntoa_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_inet_ntop_t_SOURCES = tests/portable/inet_ntop-t.c \
	tests/portable/inet_ntop.c
tests_portable_inet_ntop_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_setenv_t_SOURCES = tests/portable/setenv-t.c \
	tests/portable/setenv.c
tests_portable_setenv_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_snprintf_t_SOURCES = tests/portable/snprintf-t.c \
	tests/portable/snprintf.c
tests_portable_snprintf_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_strlcat_t_SOURCES = tests/portable/strlcat-t.c \
	tests/portable/strlcat.c
tests_portable_strlcat_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_portable_strlcpy_t_SOURCES = tests/portable/strlcpy-t.c \
	tests/portable/strlcpy.c
tests_portable_strlcpy_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_concat_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_fdflag_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_messages_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_network_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_vector_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a
tests_util_xmalloc_LDADD = util/libutil.a portable/libportable.a
tests_util_xwrite_t_SOURCES = tests/util/fakewrite.c tests/util/xwrite.c \
	tests/util/xwrite-t.c
tests_util_xwrite_t_LDADD = tests/libtest.a util/libutil.a \
	portable/libportable.a

check-local: $(check_PROGRAMS)
	set -e; if [ x"$(builddir)" != x"$(srcdir)" ] ; then \
	    for f in $(TEST_FILES) ; do \
		cp "$(srcdir)/$$f" "$(builddir)/$$f" ; \
	    done \
	fi
	cd tests && ./runtests TESTS
