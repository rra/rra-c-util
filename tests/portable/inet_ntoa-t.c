/*
 * inet_ntoa test suite.
 *
 * Copyright 2000, 2001, 2004 Russ Allbery <rra@stanford.edu>
 * Copyright 2009
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
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
