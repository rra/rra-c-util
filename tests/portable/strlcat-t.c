/*
 * strlcat test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2002, 2004 Russ Allbery <rra@stanford.edu>
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

size_t test_strlcat(char *, const char *, size_t);


int
main(void)
{
    char buffer[10] = "";

    plan(27);

    is_int(3, test_strlcat(buffer, "foo", sizeof(buffer)),
           "strlcat into empty buffer");
    is_string("foo", buffer, "...with right output");
    is_int(7, test_strlcat(buffer, " bar", sizeof(buffer)),
           "...and append more");
    is_string("foo bar", buffer, "...and output is still correct");
    is_int(9, test_strlcat(buffer, "!!", sizeof(buffer)),
           "...and append to buffer limit");
    is_string("foo bar!!", buffer, "...output is still correct");
    is_int(10, test_strlcat(buffer, "!", sizeof(buffer)),
           "...append one more character");
    is_string("foo bar!!", buffer, "...and output didn't change");
    ok(buffer[9] == '\0', "...buffer still nul-terminated");
    buffer[0] = '\0';
    is_int(11, test_strlcat(buffer, "hello world", sizeof(buffer)),
           "append single long string");
    is_string("hello wor", buffer, "...string truncates properly");
    ok(buffer[9] == '\0', "...buffer still nul-terminated");
    buffer[0] = '\0';
    is_int(7, test_strlcat(buffer, "sausage", 5), "lie about buffer length");
    is_string("saus", buffer, "...contents are correct");
    is_int(14, test_strlcat(buffer, "bacon eggs", sizeof(buffer)),
           "...add more up to real size");
    is_string("sausbacon", buffer, "...and result is correct");

    /* Make sure that with a size of 0, the destination isn't changed. */
    is_int(11, test_strlcat(buffer, "!!", 0), "no change with size of 0");
    is_string("sausbacon", buffer, "...and content is the same");

    /* Now play with empty strings. */
    is_int(9, test_strlcat(buffer, "", 0),
           "correct count when appending empty string");
    is_string("sausbacon", buffer, "...and contents are unchanged");
    buffer[0] = '\0';
    is_int(0, test_strlcat(buffer, "", sizeof(buffer)),
           "correct count when appending empty string to empty buffer");
    is_string("", buffer, "...and buffer content is correct");
    is_int(3, test_strlcat(buffer, "foo", 2), "append to length 2 buffer");
    is_string("f", buffer, "...and got only a single character");
    ok(buffer[1] == '\0', "...and buffer is still nul-terminated");
    is_int(1, test_strlcat(buffer, "", sizeof(buffer)),
           "append an empty string");
    ok(buffer[1] == '\0', "...and buffer is still nul-terminated");

    return 0;
}
