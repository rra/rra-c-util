/*
 * PAM utility argument initialization test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010 Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/pam.h>
#include <portable/system.h>

#include <pam-util/args.h>
#include <tests/fakepam/testing.h>
#include <tests/tap/basic.h>


int
main(void)
{
    pam_handle_t *pamh;
    struct pam_conv conv = { NULL, NULL };
    struct pam_args *args;

    plan(11);

    if (pam_start("test", NULL, &conv, &pamh) != PAM_SUCCESS)
        sysbail("Fake PAM initialization failed");
    args = putil_args_new(pamh, 0);
    ok(args != NULL, "New args struct is not NULL");
    ok(args->pamh == pamh, "...and pamh is correct");
    ok(args->config == NULL, "...and config is NULL");
    ok(args->user == NULL, "...and user is NULL");
    is_int(args->debug, false, "...and debug is false");
    is_int(args->silent, false, "...and silent is false");
#ifdef HAVE_KERBEROS
    ok(args->ctx != NULL, "...and the Kerberos context is initialized");
    ok(args->realm == NULL, "...and realm is NULL");
#else
    skip_block(2, "Kerberos support not configured");
#endif
    putil_args_free(args);
    ok(1, "Freeing the args struct works");

    args = putil_args_new(pamh, PAM_SILENT);
    ok(args != NULL, "New args struct with PAM_SILENT is not NULL");
    is_int(args->silent, true, "...and silent is true");
    putil_args_free(args);

    return 0;
}
