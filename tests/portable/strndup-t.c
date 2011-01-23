/*
 * strndup test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 *
 * The authors hereby relinquish any claim to any copyright that they may have
 * in this work, whether granted under contract or by operation of law or
 * international treaty, and hereby commit to the public, at large, that they
 * shall not, at any time in the future, seek to enforce any copyright in this
 * work against any person or entity, or prevent any person or entity from
 * copying, publishing, distributing or creating derivative works of this
 * work.
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
