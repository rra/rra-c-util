/*
 * inet_ntop test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2005 Russ Allbery <rra@stanford.edu>
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

#include <errno.h>

#include <tests/tap/basic.h>

/* Some systems too old to have inet_ntop don't have EAFNOSUPPORT. */
#ifndef EAFNOSUPPORT
# define EAFNOSUPPORT EDOM
#endif

const char *test_inet_ntop(int, const void *, char *, socklen_t);


static void
test_addr(const char *expected, unsigned long addr)
{
    struct in_addr in;
    char result[INET_ADDRSTRLEN];

    in.s_addr = htonl(addr);
    if (test_inet_ntop(AF_INET, &in, result, sizeof(result)) == NULL) {
        printf("# cannot convert %lu: %s", addr, strerror(errno));
        ok(0, "converting %s", expected);
    } else
        ok(1, "converting %s", expected);
    is_string(expected, result, "...with correct result");
}


int
main(void)
{
    plan(6 + 5 * 2);

    ok(test_inet_ntop(AF_UNIX, NULL, NULL, 0) == NULL, "AF_UNIX failure");
    is_int(EAFNOSUPPORT, errno, "...with right errno");
    ok(test_inet_ntop(AF_INET, NULL, NULL, 0) == NULL, "empty buffer");
    is_int(ENOSPC, errno, "...with right errno");
    ok(test_inet_ntop(AF_INET, NULL, NULL, 11) == NULL, "NULL buffer");
    is_int(ENOSPC, errno, "...with right errno");

    test_addr(        "0.0.0.0", 0x0);
    test_addr(      "127.0.0.0", 0x7f000000UL);
    test_addr("255.255.255.255", 0xffffffffUL);
    test_addr("172.200.232.199", 0xacc8e8c7UL);
    test_addr(        "1.2.3.4", 0x01020304UL);

    return 0;
}
