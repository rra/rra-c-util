/*
 * inet_ntoa test suite.
 *
 * Copyright (c) 2004, 2005, 2006, 2007
 *     by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *     2002, 2003 by The Internet Software Consortium and Rich Salz
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>
#include <portable/socket.h>

#include <tests/libtest.h>

const char *test_inet_ntoa(const struct in_addr);


static void
test_addr(int n, const char *expected, unsigned long addr)
{
    struct in_addr in;

    in.s_addr = htonl(addr);
    ok_string(n, expected, test_inet_ntoa(in));
}


int
main(void)
{
    test_init(5);

    test_addr(1,         "0.0.0.0", 0x0);
    test_addr(2,       "127.0.0.0", 0x7f000000UL);
    test_addr(3, "255.255.255.255", 0xffffffffUL);
    test_addr(4, "172.200.232.199", 0xacc8e8c7UL);
    test_addr(5,         "1.2.3.4", 0x01020304UL);

    return 0;
}
