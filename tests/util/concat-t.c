/*
 * concat test suite.
 *
 * Copyright (c) 2004, 2005, 2006
 *     by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *     2002, 2003 by The Internet Software Consortium and Rich Salz
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <tests/libtest.h>
#include <util/util.h>

#define END (char *) 0


/*
 * Memory leaks everywhere!  Whoo-hoo!
 */
int
main(void)
{
    test_init(13);

    ok_string( 1, "a",     concat("a",                   END));
    ok_string( 2, "ab",    concat("a", "b",              END));
    ok_string( 3, "ab",    concat("ab", "",              END));
    ok_string( 4, "ab",    concat("", "ab",              END));
    ok_string( 5, "",      concat("",                    END));
    ok_string( 6, "abcde", concat("ab", "c", "", "de",   END));
    ok_string( 7, "abcde", concat("abc", "de", END, "f", END));

    ok_string( 8, "/foo",             concatpath("/bar", "/foo"));
    ok_string( 9, "/foo/bar",         concatpath("/foo", "bar"));
    ok_string(10, "./bar",            concatpath("/foo", "./bar"));
    ok_string(11, "/bar/baz/foo/bar", concatpath("/bar/baz", "foo/bar"));
    ok_string(12, "./foo",            concatpath(NULL, "foo"));
    ok_string(13, "/foo/bar",         concatpath(NULL, "/foo/bar"));

    return 0;
}
