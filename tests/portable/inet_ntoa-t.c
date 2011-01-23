/*
 * inet_ntoa test suite.
 *
 * Copyright 2000, 2001, 2004 Russ Allbery <rra@stanford.edu>
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
#include <portable/socket.h>

#include <tests/tap/basic.h>

const char *test_inet_ntoa(const struct in_addr);


static void
test_addr(const char *expected, unsigned long addr)
{
    struct in_addr in;

    in.s_addr = htonl(addr);
    is_string(expected, test_inet_ntoa(in), "address %s", expected);
}


int
main(void)
{
    plan(5);

    test_addr(        "0.0.0.0", 0x0);
    test_addr(      "127.0.0.0", 0x7f000000UL);
    test_addr("255.255.255.255", 0xffffffffUL);
    test_addr("172.200.232.199", 0xacc8e8c7UL);
    test_addr(        "1.2.3.4", 0x01020304UL);

    return 0;
}
