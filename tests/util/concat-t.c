/*
 * concat test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2000, 2001, 2004, 2005 Russ Allbery <rra@stanford.edu>
 * Copyright 2009
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <tests/tap/basic.h>
#include <util/concat.h>

#define END (char *) 0

/*
 * Memory leaks everywhere!  Whoo-hoo!
 */
int
main(void)
{
    plan(13);

    is_string("a",     concat("a",                   END), "concat 1");
    is_string("ab",    concat("a", "b",              END), "concat 2");
    is_string("ab",    concat("ab", "",              END), "concat 3");
    is_string("ab",    concat("", "ab",              END), "concat 4");
    is_string("",      concat("",                    END), "concat 5");
    is_string("abcde", concat("ab", "c", "", "de",   END), "concat 6");
    is_string("abcde", concat("abc", "de", END, "f", END), "concat 7");

    is_string("/foo",             concatpath("/bar", "/foo"),        "path 1");
    is_string("/foo/bar",         concatpath("/foo", "bar"),         "path 2");
    is_string("./bar",            concatpath("/foo", "./bar"),       "path 3");
    is_string("/bar/baz/foo/bar", concatpath("/bar/baz", "foo/bar"), "path 4");
    is_string("./foo",            concatpath(NULL, "foo"),           "path 5");
    is_string("/foo/bar",         concatpath(NULL, "/foo/bar"),      "path 6");

    return 0;
}
