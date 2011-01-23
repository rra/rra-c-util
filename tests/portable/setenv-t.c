/*
 * setenv test suite.
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
