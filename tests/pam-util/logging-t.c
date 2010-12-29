/*
 * PAM logging test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010 Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/pam.h>
#include <portable/system.h>

#include <syslog.h>

#include <pam-util/args.h>
#include <pam-util/logging.h>
#include <tests/fakepam/testing.h>
#include <tests/tap/basic.h>

/* Test a normal PAM logging function. */
#define TEST(func, p, n)                              \
    do {                                              \
        (func)(args, "%s", "foo");                    \
        asprintf(&expected, "%d %s", (p), "foo");     \
        seen = pam_output();                          \
        is_string(expected, seen, "%s", (n));         \
        free(seen);                                   \
        free(expected);                               \
    } while (0);

/* Test a PAM error logging function. */
#define TEST_PAM(func, c, p, n)                         \
    do {                                                \
        (func)(args, (c), "%s", "bar");                 \
        asprintf(&expected, "%d %s: %s", (p), "bar",    \
                 pam_strerror(args->pamh, c));          \
        seen = pam_output();                            \
        is_string(expected, seen, "%s", (n));           \
        free(seen);                                     \
        free(expected);                                 \
    } while (0);

/* Test a PAM Kerberos error logging function .*/
#define TEST_KRB5(func, p, n)                                             \
    do {                                                                  \
        code = krb5_parse_name(args->ctx, "foo@bar@EXAMPLE.COM", &princ); \
        (func)(args, code, "%s", "krb");                                  \
        code = krb5_parse_name(args->ctx, "foo@bar@EXAMPLE.COM", &princ); \
        asprintf(&expected, "%d %s: %s", (p), "krb",                      \
                 krb5_get_error_message(args->ctx, code));                \
        seen = pam_output();                                              \
        is_string(expected, seen, "%s", (n));                             \
        free(seen);                                                       \
        free(expected);                                                   \
    } while (0);


int
main(void)
{
    pam_handle_t *pamh;
    struct pam_args *args;
    struct pam_conv conv = { NULL, NULL };
    char *expected, *seen;
#ifdef HAVE_KERBEROS
    krb5_error_code code;
    krb5_principal princ;
#endif

    plan(13);

    if (pam_start("test", NULL, &conv, &pamh) != PAM_SUCCESS)
        sysbail("Fake PAM initialization failed");
    args = putil_args_new(pamh, 0);
    TEST(putil_crit,  LOG_CRIT,  "putil_crit");
    TEST(putil_err,   LOG_ERR,   "putil_err");
    putil_debug(args, "%s", "foo");
    ok(pam_output() == NULL, "putil_debug without debug on");
    args->debug = true;
    TEST(putil_debug, LOG_DEBUG, "putil_debug");
    args->debug = false;

    TEST_PAM(putil_crit_pam,  PAM_SYSTEM_ERR, LOG_CRIT,  "putil_crit_pam S");
    TEST_PAM(putil_crit_pam,  PAM_BUF_ERR,    LOG_CRIT,  "putil_crit_pam B");
    TEST_PAM(putil_err_pam,   PAM_SYSTEM_ERR, LOG_ERR,   "putil_err_pam");
    putil_debug_pam(args, PAM_SYSTEM_ERR, "%s", "bar");
    ok(pam_output() == NULL, "putil_debug_pam without debug on");
    args->debug = true;
    TEST_PAM(putil_debug_pam, PAM_SYSTEM_ERR, LOG_DEBUG, "putil_debug_pam");
    args->debug = false;

#ifdef HAVE_KERBEROS
    TEST_KRB5(putil_crit_krb5,  LOG_CRIT,  "putil_crit_krb5");
    TEST_KRB5(putil_err_krb5,   LOG_ERR,   "putil_err_krb5");
    code = krb5_parse_name(args->ctx, "foo@bar@EXAMPLE.COM", &princ);
    putil_debug_krb5(args, code, "%s", "krb");
    ok(pam_output() == NULL, "putil_debug_krb5 without debug on");
    args->debug = true;
    TEST_KRB5(putil_debug_krb5, LOG_DEBUG, "putil_debug_krb5");
    args->debug = false;
#else
    skip_block(4, "not built with Kerberos support");
#endif

    return 0;
}
