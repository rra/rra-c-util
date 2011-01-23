/*
 * strndup test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2011
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
