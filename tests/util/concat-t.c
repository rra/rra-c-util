/*
 * concat test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2000, 2001, 2004, 2005 Russ Allbery <rra@stanford.edu>
 * Copyright 2009
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
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
