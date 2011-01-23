/*
 * strndup test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2011
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <tests/tap/basic.h>

char *test_strndup(const char *, size_t);


int
main(void)
{
    char *result = NULL;

    plan(4);

    result = strndup("foo", 8);
    is_string("foo", result, "strndup longer than string");
    free(result);
    result = strndup("foo", 2);
    is_string("fo", result, "strndup shorter than string");
    free(result);
    result = strndup("foo", 3);
    is_string("foo", result, "strndup same size as string");
    free(result);
    result = strndup("foo", 0);
    is_string("", result, "strndup of size 0");
    free(result);

    return 0;
}
