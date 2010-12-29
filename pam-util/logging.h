/*
 * Interface to standard PAM logging.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2008, 2009, 2010
 *     Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#ifndef PAM_UTIL_LOGGING_H
#define PAM_UTIL_LOGGING_H 1

#include <config.h>
#include <portable/macros.h>
#ifdef HAVE_KERBEROS
# include <portable/krb5.h>
#endif
#include <portable/pam.h>
#include <syslog.h>

/* Forward declarations to avoid extra includes. */
struct pam_args;

BEGIN_DECLS

/* Default to a hidden visibility for all internal functions. */
#pragma GCC visibility push(hidden)

/*
 * Error reporting and debugging functions.  For each log level, there are
 * two functions.  The _log function just prints out the message it's given.
 * The _log_pam function reports a PAM error using pam_strerror.
 */
void putil_crit(struct pam_args *, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));
void putil_crit_pam(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_err(struct pam_args *, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));
void putil_err_pam(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_notice(struct pam_args *, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));
void putil_notice_pam(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_debug(struct pam_args *, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));
void putil_debug_pam(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));

/*
 * The Kerberos versions of the PAM logging and debugging functions, which
 * report the last Kerberos error.  These are only available if built with
 * Kerberos support.
 */
#ifdef HAVE_KERBEROS
void putil_crit_krb5(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_err_krb5(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_notice_krb5(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
void putil_debug_krb5(struct pam_args *, int, const char *, ...)
    __attribute__((__format__(printf, 3, 4)));
#endif

/* Log an authentication failure. */
void putil_log_failure(struct pam_args *, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));

/* Undo default visibility change. */
#pragma GCC visibility pop

END_DECLS

/* __func__ is C99, but not provided by all implementations. */
#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

/* Macros to record entry and exit from the main PAM functions. */
#define ENTRY(args, flags)                                              \
    if (args->debug)                                                    \
        pam_syslog((args)->pamh, LOG_DEBUG,                             \
                   "%s: entry (0x%x)", __func__, (flags))
#define EXIT(args, pamret)                                              \
    if (args->debug)                                                    \
        pam_syslog((args)->pamh, LOG_DEBUG, "%s: exit (%s)", __func__,  \
                   ((pamret) == PAM_SUCCESS) ? "success"                \
                   : (((pamret) == PAM_IGNORE) ? "ignore" : "failure"))

#endif /* !PAM_UTIL_LOGGING_H */
