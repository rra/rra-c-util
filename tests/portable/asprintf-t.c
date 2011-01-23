/*
 * asprintf and vasprintf test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2008, 2009
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

int test_asprintf(char **, const char *, ...)
    __attribute__((__format__(printf, 2, 3)));
int test_vasprintf(char **, const char *, va_list);

static int
vatest(char **result, const char *format, ...)
{
    va_list args;
    int status;

    va_start(args, format);
    status = test_vasprintf(result, format, args);
    va_end(args);
    return status;
}

int
main(void)
{
    char *result = NULL;

    plan(12);

    is_int(7, test_asprintf(&result, "%s", "testing"), "asprintf length");
    is_string("testing", result, "asprintf result");
    free(result);
    ok(3, "free asprintf");
    is_int(0, test_asprintf(&result, "%s", ""), "asprintf empty length");
    is_string("", result, "asprintf empty string");
    free(result);
    ok(6, "free asprintf of empty string");

    is_int(6, vatest(&result, "%d %s", 2, "test"), "vasprintf length");
    is_string("2 test", result, "vasprintf result");
    free(result);
    ok(9, "free vasprintf");
    is_int(0, vatest(&result, "%s", ""), "vasprintf empty length");
    is_string("", result, "vasprintf empty string");
    free(result);
    ok(12, "free vasprintf of empty string");

    return 0;
}
