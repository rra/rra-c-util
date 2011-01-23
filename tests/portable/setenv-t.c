/*
 * setenv test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2000, 2001, 2004, 2005 Russ Allbery <rra@stanford.edu>
 * Copyright 2009 Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <errno.h>

#include <tests/tap/basic.h>

int test_setenv(const char *name, const char *value, int overwrite);

static const char test_var[] = "SETENV_TEST";
static const char test_value1[] = "Do not taunt Happy Fun Ball.";
static const char test_value2[] = "Do not use Happy Fun Ball on concrete.";


int
main(void)
{
    plan(8);

    if (getenv(test_var))
        bail("%s already in the environment!", test_var);

    ok(test_setenv(test_var, test_value1, 0) == 0, "set string 1");
    is_string(test_value1, getenv(test_var), "...and getenv correct");
    ok(test_setenv(test_var, test_value2, 0) == 0, "set string 2");
    is_string(test_value1, getenv(test_var), "...and getenv unchanged");
    ok(test_setenv(test_var, test_value2, 1) == 0, "overwrite string 2");
    is_string(test_value2, getenv(test_var), "...and getenv changed");
    ok(test_setenv(test_var, "", 1) == 0, "overwrite with empty string");
    is_string("", getenv(test_var), "...and getenv correct");

    return 0;
}
