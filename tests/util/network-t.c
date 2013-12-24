/*
 * network test suite.
 *
 * The canonical version of this file is maintained in the rra-c-util package,
 * which can be found at <http://www.eyrie.org/~eagle/software/rra-c-util/>.
 *
 * Written by Russ Allbery <eagle@eyrie.org>
 * Copyright 2005 Russ Allbery <eagle@eyrie.org>
 * Copyright 2009, 2010, 2011, 2012, 2013
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

#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#include <tests/tap/basic.h>
#include <util/macros.h>
#include <util/messages.h>
#include <util/network.h>

/* Set this globally to false if IPv6 is supported but doesn't work. */
static bool ipv6_works = true;


/*
 * Check whether IPv6 actually works.  On some systems (such as Solaris 8
 * without IPv6 configured), it's possible to create an IPv6 socket, but
 * binding the socket will fail.  We therefore attempt an IPv6 socket creation
 * and, if it fails, check errno for several errors that indicate that IPv6
 * doesn't work.  If it doesn't, we set the ipv6 global so that subsequent
 * tests can be skipped.
 */
static void
check_ipv6(void)
{
    socket_type fd;

    /* Create the socket.  If this works, ipv6 is supported. */
    fd = network_bind_ipv6(SOCK_STREAM, "::1", 11119);
    if (fd != INVALID_SOCKET) {
        close(fd);
        return;
    }

    /* IPv6 not recognized, indicating no support. */
    if (errno == EAFNOSUPPORT || errno == EPROTONOSUPPORT)
        ipv6_works = false;

    /* IPv6 is recognized but we can't actually use it. */
    if (errno == EADDRNOTAVAIL)
        ipv6_works = false;
}


/*
 * A client writer used to generate data for a server test.  Connect to the
 * given host on port 11119 and send a constant string to a socket.  Takes the
 * source address as well to pass into network_connect_host.  If the flag is
 * true, expects to succeed in connecting; otherwise, fail the test (by
 * exiting with a non-zero status) if the connection is successful.
 *
 * If the succeed argument is true, this is guarateed to never return.
 */
static void
client_writer(const char *host, const char *source, bool succeed)
{
    socket_type fd;
    FILE *out;

    fd = network_connect_host(host, 11119, source, 0);
    if (fd == INVALID_SOCKET) {
        if (succeed)
            _exit(1);
        else
            return;
    }
    out = fdopen(fd, "w");
    if (out == NULL)
        sysdie("fdopen failed");
    fputs("socket test\r\n", out);
    fclose(out);
    _exit(succeed ? 0 : 1);
}


/*
 * Like client_writer, but uses network_client_create and then manual socket
 * connection code instead of network_connect_host, always connects to IPv4
 * localhost, and expects to always succeed on the connection.
 */
static void
client_create_writer(const char *source)
{
    socket_type fd;
    struct sockaddr_in sin;
    FILE *out;

    fd = network_client_create(PF_INET, SOCK_STREAM, source);
    if (fd == INVALID_SOCKET)
        _exit(1);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(11119);
    sin.sin_addr.s_addr = htonl(0x7f000001UL);
    if (connect(fd, (struct sockaddr *) &sin, sizeof(sin)) < 0)
        _exit(1);
    out = fdopen(fd, "w");
    if (out == NULL)
        _exit(1);
    fputs("socket test\r\n", out);
    fclose(out);
    _exit(0);
}


/*
 * Used to test network_read.  Connects, sends a couple of strings, then
 * sleeps for 10 seconds before sending another string so that timeouts can be
 * tested.  Meant to be run in a child process.
 */
static void
client_delay_writer(const char *host)
{
    socket_type fd;

    fd = network_connect_host(host, 11119, NULL, 0);
    if (fd == INVALID_SOCKET)
        _exit(1);
    if (socket_write(fd, "one\n", 4) != 4)
        _exit(1);
    if (socket_write(fd, "two\n", 4) != 4)
        _exit(1);
    sleep(10);
    if (socket_write(fd, "three\n", 6) != 6)
        _exit(1);
    _exit(0);
}


/*
 * Used to test network_write.  Connects, reads 64KB from the network, then
 * sleeps before reading another 64KB.  Meant to be run in a child process.
 */
static void
client_delay_reader(const char *host)
{
    char *buffer;
    socket_type fd;

    fd = network_connect_host(host, 11119, NULL, 0);
    if (fd == INVALID_SOCKET)
        _exit(1);
    buffer = malloc(64 * 1024);
    if (buffer == NULL)
        _exit(1);
    if (!network_read(fd, buffer, 64 * 1024, 0))
        _exit(1);
    sleep(10);
    if (!network_read(fd, buffer, 64 * 1024, 0))
        _exit(1);
    free(buffer);
    _exit(0);
}


/*
 * When testing the bind (server) functions, we create listening sockets, fork
 * a child process to connect to it, and accept the connection and read the
 * data in the server.  The test reporting is therefore done by the listener.
 * There are two listeners, depending on whether we're listening to a single
 * socket or an array of sockets, both of which invoke this handler when the
 * connection is accepted.
 *
 * Check that the result of accept, read data from the client, and ensure we
 * got the expected data, reporting all results through the normal test
 * reporting mechanism.
 */
static void
test_server_connection(socket_type client)
{
    FILE *out;
    char buffer[512];

    /* Verify that the result of accept is good. */
    if (client == INVALID_SOCKET) {
        sysdiag("cannot accept connection from socket");
        ok_block(2, 0, "...socket read test");
        return;
    }
    ok(1, "...socket accept");

    /* Read data from the client and ensure it matches our expectations. */
    out = fdopen(client, "r");
    if (fgets(buffer, sizeof(buffer), out) == NULL) {
        sysdiag("cannot read from socket");
        ok(0, "...socket read");
    }
    is_string("socket test\r\n", buffer, "...socket read");
    fclose(out);
}


/*
 * Test a single listening socket.  Accepts one connection and invokes
 * test_server_connection.  For skipping purposes, this produces two tests.
 */
static void
test_server_accept(socket_type fd)
{
    socket_type client;

    client = accept(fd, NULL, NULL);
    test_server_connection(client);
    socket_close(fd);
}


/*
 * A varient version of the server portion of the test.  Takes an array of
 * sockets and the size of the sockets and accepts a connection on any of
 * those sockets.  Ensures that the client address information is stored
 * correctly by checking that it is set to IPv4 localhost.  For skipping
 * purposes, this produces four tests.
 *
 * saddr is allocated from the heap instead of using a local struct
 * sockaddr_storage to work around a misdiagnosis of strict aliasing
 * violations from gcc 4.4 (fixed in later versions).
 */
static void
test_server_accept_any(socket_type fds[], unsigned int count)
{
    socket_type client;
    unsigned int i;
    struct sockaddr *saddr;
    socklen_t slen;

    slen = sizeof(struct sockaddr_storage);
    saddr = bcalloc(1, slen);
    client = network_accept_any(fds, count, saddr, &slen);
    test_server_connection(client);
    is_int(AF_INET, saddr->sa_family, "...address family is IPv4");
    is_int(htonl(0x7f000001UL),
           ((struct sockaddr_in *) (void *) saddr)->sin_addr.s_addr,
           "...and client address is 127.0.0.1");
    free(saddr);
    for (i = 0; i < count; i++)
        socket_close(fds[i]);
}


/*
 * Bring up a server on port 11119 on the loopback address and test connecting
 * to it via IPv4.  Takes an optional source address to use for client
 * connections.  For skipping purposes, this produces four tests.
 */
static void
test_ipv4(const char *source)
{
    socket_type fd;
    pid_t child;
    int status;

    /* Set up the server socket. */
    fd = network_bind_ipv4(SOCK_STREAM, "127.0.0.1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create or bind socket");
    ok(fd != INVALID_SOCKET, "IPv4 server test");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /* Fork off a child writer and test the server accept. */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0) {
        socket_close(fd);
        client_writer("127.0.0.1", source, true);
    } else {
        test_server_accept(fd);
        waitpid(child, &status, 0);
        is_int(0, status, "client made correct connections");
    }
}


/*
 * Bring up a server on port 11119 on the loopback address and test connecting
 * to it via IPv6.  Takes an optional source address to use for client
 * connections.  For skipping purposes, this produces four tests.
 */
#ifdef HAVE_INET6

static void
test_ipv6(const char *source)
{
    socket_type fd;
    pid_t child;
    int status;

    /* Set up the server socket. */
    fd = network_bind_ipv6(SOCK_STREAM, "::1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create socket");
    ok(fd != INVALID_SOCKET, "IPv6 server test");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /*
     * Fork off a child writer and test the server accept.  If IPV6_V6ONLY is
     * supported, we can also check that connecting to 127.0.0.1 will fail.
     */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0) {
        socket_close(fd);
#ifdef IPV6_V6ONLY
        client_writer("127.0.0.1", NULL, false);
#endif
        client_writer("::1", source, true);
    } else {
        test_server_accept(fd);
        waitpid(child, &status, 0);
        is_int(0, status, "client made correct connections");
    }
}

#else /* !HAVE_INET6 */

static void
test_ipv6(const char *source UNUSED)
{
    skip_block(4, "IPv6 not supported");
}

#endif /* !HAVE_INET6 */


/*
 * Returns the struct sockaddr * corresponding to a local socket.  Handles the
 * initial allocation being too small and dynamically increasing it.  Caller
 * is responsible for freeing the allocated sockaddr.
 */
static struct sockaddr *
get_sockaddr(socket_type fd)
{
    struct sockaddr *saddr;
    socklen_t size;

    saddr = bmalloc(sizeof(struct sockaddr_storage));
    size = sizeof(struct sockaddr_storage);
    if (getsockname(fd, saddr, &size) < 0)
        sysbail("cannot getsockname");
    if (size > sizeof(struct sockaddr)) {
        free(saddr);
        saddr = bmalloc(size);
        if (getsockname(fd, saddr, &size) < 0)
            sysbail("cannot getsockname");
    }
    return saddr;
}


/*
 * Bring up a server on port 11119 on all addresses and try connecting to it
 * via all of the available protocols.  Takes an optional source address to
 * use for client connections.  For skipping purposes, this produces eight
 * tests.
 */
static void
test_all(const char *source_ipv4, const char *source_ipv6 UNUSED)
{
    socket_type *fds, fd;
    unsigned int count, i;
    pid_t child;
    struct sockaddr *saddr;
    int status;

    /* Bind sockets for all available local addresses. */
    if (!network_bind_all(SOCK_STREAM, 11119, &fds, &count))
        sysbail("cannot create or bind socket");

    /*
     * There should be at most two, one for IPv4 and one for IPv6, but allow
     * for possible future weirdness in networking.
     */
    if (count > 2) {
        diag("got more than two sockets, using just the first two");
        count = 2;
    }

    /* We'll test each socket in turn by listening and trying to connect. */
    for (i = 0; i < count; i++) {
        fd = fds[i];
        if (listen(fd, 1) < 0)
            sysbail("cannot listen to socket %d", fd);
        ok(fd != INVALID_SOCKET, "all address server test (part %d)", i + 1);

        /* Get the socket type to determine what type of client to run. */
        saddr = get_sockaddr(fd);

        /*
         * Fork off a child writer and test the server accept.  If IPV6_V6ONLY
         * is supported, we can also check that connecting to 127.0.0.1 will
         * fail.
         */
        child = fork();
        if (child < 0)
            sysbail("cannot fork");
        else if (child == 0) {
            if (saddr->sa_family == AF_INET) {
                client_writer("::1", source_ipv6, false);
                client_writer("127.0.0.1", source_ipv4, true);
#ifdef HAVE_INET6
            } else if (saddr->sa_family == AF_INET6) {
# ifdef IPV6_V6ONLY
                client_writer("127.0.0.1", source_ipv4, false);
# endif
                client_writer("::1", source_ipv6, true);
#endif
            }
        } else {
            test_server_accept(fd);
            waitpid(child, &status, 0);
            is_int(0, status, "client made correct connections");
        }
        free(saddr);
    }
    network_bind_all_free(fds);

    /* If we only got one listening socket, skip for consistent test count. */
    if (count == 1)
        skip_block(4, "only one listening socket");
}


/*
 * Bring up a server on port 11119 on the loopback address and test connecting
 * to it via IPv4 using network_client_create.  Takes an optional source
 * address to use for client connections.
 */
static void
test_create_ipv4(const char *source)
{
    socket_type fd;
    pid_t child;
    int status;

    /* Create the socket and listen to it. */
    fd = network_bind_ipv4(SOCK_STREAM, "127.0.0.1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create or bind socket");
    ok(fd != INVALID_SOCKET, "IPv4 network client");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /* Fork off a child that uses network_client_create. */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0)
        client_create_writer(source);
    else {
        test_server_accept(fd);
        waitpid(child, &status, 0);
        is_int(0, status, "client made correct connections");
    }
}


/*
 * Bring up a server on port 11119 on all addresses and try connecting to it
 * via 127.0.0.1, using network_accept_any underneath.  For skipping purposes,
 * this runs three tests.
 */
static void
test_any(void)
{
    socket_type *fds;
    unsigned int count, i;
    pid_t child;
    int status;

    if (!network_bind_all(SOCK_STREAM, 11119, &fds, &count))
        sysbail("cannot create or bind socket");
    for (i = 0; i < count; i++)
        if (listen(fds[i], 1) < 0)
            sysbail("cannot listen to socket %d", fds[i]);
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0)
        client_writer("127.0.0.1", NULL, true);
    else {
        test_server_accept_any(fds, count);
        waitpid(child, &status, 0);
        is_int(0, status, "client made correct connections");
    }
    network_bind_all_free(fds);
}


/*
 * Test connect timeouts using IPv4.  Bring up a server on port 11119 on the
 * loopback address and test connections to it.  The server only accepts one
 * connection at a time, so a subsequent connection will time out.
 */
static void
test_timeout_ipv4(void)
{
    socket_type fd, c;
    pid_t child;
    socket_type block[20];
    int i, err;

    /*
     * Create the listening socket.  We set the listening queue size to 1,
     * but some operating systems, including Linux, will allow more
     * connection attempts to succeed than the backlog size.  We'll therefore
     * have to hammer this server with connections to try to get it to fail.
     */
    fd = network_bind_ipv4(SOCK_STREAM, "127.0.0.1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create or bind socket");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /* Fork off a child that just runs accept once and then sleeps. */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0) {
        alarm(10);
        c = accept(fd, NULL, 0);
        if (c == INVALID_SOCKET)
            _exit(1);
        sleep(9);
        _exit(0);
    }

    /* In the parent.  Open that first connection. */
    socket_close(fd);
    c = network_connect_host("127.0.0.1", 11119, NULL, 1);
    ok(c != INVALID_SOCKET, "Timeout: first connection worked");

    /*
     * It can take up to fifteen connections on Linux before connections start
     * actually timing out, and sometimes they never do.
     */
    alarm(20);
    for (i = 0; i < (int) ARRAY_SIZE(block); i++) {
        block[i] = network_connect_host("127.0.0.1", 11119, NULL, 1);
        if (block[i] == INVALID_SOCKET)
            break;
    }
    err = socket_errno;

    /*
     * If we reached the end of the array, we can't force a connection
     * timeout, so just skip this test.  It's also possible that the
     * connection will fail with ECONNRESET or ECONNREFUSED if the nine second
     * sleep in the child passed, so skip in that case as well.  Otherwise,
     * expect a failure due to timeout in a reasonable amount of time (less
     * than our 20-second alarm).
     */
    if (i == ARRAY_SIZE(block))
        skip_block(2, "short listen queue does not prevent connections");
    else {
        diag("Finally timed out on socket %d", i);
        ok(block[i] == INVALID_SOCKET, "Later connection timed out");
        if (err == ECONNRESET || err == ECONNREFUSED)
            skip("connections rejected without timeout");
        else
            is_int(ETIMEDOUT, err, "...with correct error code");
    }
    alarm(0);

    /* Shut down the client and clean up resources. */
    kill(child, SIGTERM);
    waitpid(child, NULL, 0);
    socket_close(c);
    for (i--; i >= 0; i--)
        if (block[i] != INVALID_SOCKET)
            socket_close(block[i]);
    socket_close(fd);
}


/*
 * Test the network read function with a timeout.  We fork off a child process
 * that runs delay_writer, and then we read from the network twice, once with
 * a timeout and once without, and then try a third time when we should time
 * out.
 */
static void
test_network_read(void)
{
    socket_type fd, c;
    pid_t child;
    struct sockaddr_in sin;
    socklen_t slen;
    char buffer[4];

    /* Create the listening socket. */
    fd = network_bind_ipv4(SOCK_STREAM, "127.0.0.1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create or bind socket");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /* Fork off a child process that writes some data with delays. */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0) {
        socket_close(fd);
        client_delay_writer("127.0.0.1");
    }

    /* Set an alarm just in case our timeouts don't work. */
    alarm(10);

    /* Accept the client connection. */
    slen = sizeof(sin);
    c = accept(fd, &sin, &slen);
    if (c == INVALID_SOCKET)
        sysbail("cannot accept on socket");

    /* Now test a couple of simple reads, with and without timeout. */
    socket_set_errno(0);
    ok(network_read(c, buffer, sizeof(buffer), 0), "network_read");
    ok(memcmp("one\n", buffer, sizeof(buffer)) == 0, "...with good data");
    ok(network_read(c, buffer, sizeof(buffer), 1),
       "network_read with timeout");
    ok(memcmp("two\n", buffer, sizeof(buffer)) == 0, "...with good data");

    /*
     * The third read should abort with a timeout, since the writer is writing
     * with a ten second delay.
     */
    ok(!network_read(c, buffer, sizeof(buffer), 1),
       "network_read aborted with timeout");
    is_int(ETIMEDOUT, socket_errno, "...with correct error");
    ok(memcmp("two\n", buffer, sizeof(buffer)) == 0, "...and data unchanged");
    alarm(0);

    /* Clean up. */
    socket_close(c);
    kill(child, SIGTERM);
    waitpid(child, NULL, 0);
    socket_close(fd);
}


/*
 * Test the network write function with a timeout.  We fork off a child
 * process that runs delay_reader, and then we write 64KB to the network in
 * two chunks, once with a timeout and once without, and then try a third time
 * when we should time out.
 */
static void
test_network_write(void)
{
    socket_type fd, c;
    pid_t child;
    struct sockaddr_in sin;
    socklen_t slen;
    char *buffer;

    /* Create the data that we're going to send. */
    buffer = bmalloc(4096 * 1024);
    memset(buffer, 'a', 4096 * 1024);

    /* Create the listening socket. */
    fd = network_bind_ipv4(SOCK_STREAM, "127.0.0.1", 11119);
    if (fd == INVALID_SOCKET)
        sysbail("cannot create or bind socket");
    if (listen(fd, 1) < 0)
        sysbail("cannot listen to socket");

    /* Create the child, which will connect and then read data with delay. */
    child = fork();
    if (child < 0)
        sysbail("cannot fork");
    else if (child == 0) {
        socket_close(fd);
        client_delay_reader("127.0.0.1");
    }

    /* Set an alarm just in case our timeouts don't work. */
    alarm(10);

    /* Accept the client connection. */
    slen = sizeof(struct sockaddr_in);
    c = accept(fd, &sin, &slen);
    if (c == INVALID_SOCKET)
        sysbail("cannot accept on socket");

    /* Test some successful writes with and without a timeout. */
    socket_set_errno(0);
    ok(network_write(c, buffer, 32 * 1024, 0), "network_write");
    ok(network_write(c, buffer, 32 * 1024, 1),
       "network_write with timeout");

    /*
     * A longer write cannot be completely absorbed before the client sleep,
     * so should fail with a timeout.
     */
    ok(!network_write(c, buffer, 4096 * 1024, 1),
       "network_write aborted with timeout");
    is_int(ETIMEDOUT, socket_errno, "...with correct error");
    alarm(0);

    /* Clean up. */
    socket_close(c);
    kill(child, SIGTERM);
    waitpid(child, NULL, 0);
    socket_close(fd);
    free(buffer);
}


/*
 * Tests network_addr_compare.  Takes the expected result, the two addresses,
 * and the mask.
 */
static void
ok_addr(int expected, const char *a, const char *b, const char *mask)
{
    const char *smask = (mask == NULL) ? "(null)" : mask;

    if (expected)
        ok(network_addr_match(a, b, mask), "compare %s %s %s", a, b, smask);
    else
        ok(!network_addr_match(a, b, mask), "compare %s %s %s", a, b, smask);
}


int
main(void)
{
    int status;
    struct addrinfo *ai, *ai4;
    struct addrinfo hints;
    char addr[INET6_ADDRSTRLEN];
    static const char *port = "119";

#ifdef HAVE_INET6
    struct addrinfo *ai6;
    char *p;
    static const char *ipv6_addr = "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210";
#endif

    /* Set up the plan. */
    plan(116);

    /*
     * Test whether IPv6 is available and actually works.  Sets the global
     * ipv6 variable, which is probed to determine whether to do IPv6 checks.
     * If IPv6 support appears to be available but doesn't work, we also have
     * to skip the test_all tests since they'll create a socket that we then
     * can't connect to.
     */
    check_ipv6();

    /* Test network_bind functions. */
    test_ipv4(NULL);
    test_ipv4("127.0.0.1");
    if (ipv6_works) {
        test_ipv6(NULL);
        test_ipv6("::1");
        test_all(NULL, NULL);
        test_all("127.0.0.1", "::1");
    } else {
        skip_block(24, "IPv6 not configured");
    }

    /* Test network_client_create. */
    test_create_ipv4(NULL);
    test_create_ipv4("127.0.0.1");

    /* Test network_accept_any. */
    test_any();

    /* Test network_connect with a timeout. */
    test_timeout_ipv4();

    /* Test network_read and network_write. */
    test_network_read();
    test_network_write();

    /*
     * Now, test network_sockaddr_sprint, network_sockaddr_equal, and
     * network_sockaddr_port.
     */
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo("127.0.0.1", port, &hints, &ai4);
    if (status != 0)
        bail("getaddrinfo on 127.0.0.1 failed: %s", gai_strerror(status));
    ok(network_sockaddr_sprint(addr, sizeof(addr), ai4->ai_addr),
       "sprint of 127.0.0.1");
    is_string("127.0.0.1", addr, "...with right results");
    is_int(119, network_sockaddr_port(ai4->ai_addr),
           "sockaddr_port");
    ok(network_sockaddr_equal(ai4->ai_addr, ai4->ai_addr), "sockaddr_equal");
    status = getaddrinfo("127.0.0.2", NULL, &hints, &ai);
    if (status != 0)
        bail("getaddrinfo on 127.0.0.2 failed: %s", gai_strerror(status));
    ok(!network_sockaddr_equal(ai->ai_addr, ai4->ai_addr),
       "sockaddr_equal of unequal addresses");
    ok(!network_sockaddr_equal(ai4->ai_addr, ai->ai_addr),
       "...and the other way around");

    /* The same for IPv6. */
#ifdef HAVE_INET6
    status = getaddrinfo(ipv6_addr, port, &hints, &ai6);
    if (status != 0)
        bail("getaddr on %s failed: %s", ipv6_addr, gai_strerror(status));
    ok(network_sockaddr_sprint(addr, sizeof(addr), ai6->ai_addr),
       "sprint of IPv6 address");
    for (p = addr; *p != '\0'; p++)
        if (islower((unsigned char) *p))
            *p = toupper((unsigned char) *p);
    is_string(ipv6_addr, addr, "...with right results");
    is_int(119, network_sockaddr_port(ai6->ai_addr), "sockaddr_port IPv6");
    ok(network_sockaddr_equal(ai6->ai_addr, ai6->ai_addr),
       "sockaddr_equal IPv6");
    ok(!network_sockaddr_equal(ai4->ai_addr, ai6->ai_addr),
       "...and not equal to IPv4");
    ok(!network_sockaddr_equal(ai6->ai_addr, ai4->ai_addr),
       "...other way around");
    freeaddrinfo(ai6);

    /* Test IPv4 mapped addresses. */
    status = getaddrinfo("::ffff:7f00:1", NULL, &hints, &ai6);
    if (status != 0)
        bail("getaddr on ::ffff:7f00:1 failed: %s", gai_strerror(status));
    ok(network_sockaddr_sprint(addr, sizeof(addr), ai6->ai_addr),
       "sprint of IPv4-mapped address");
    is_string("127.0.0.1", addr, "...with right IPv4 result");
    ok(network_sockaddr_equal(ai4->ai_addr, ai6->ai_addr),
       "sockaddr_equal of IPv4-mapped address");
    ok(network_sockaddr_equal(ai6->ai_addr, ai4->ai_addr),
       "...and other way around");
    ok(!network_sockaddr_equal(ai->ai_addr, ai6->ai_addr),
       "...but not some other address");
    ok(!network_sockaddr_equal(ai6->ai_addr, ai->ai_addr),
       "...and the other way around");
    freeaddrinfo(ai6);
#else
    skip_block(12, "IPv6 not supported");
#endif
    freeaddrinfo(ai);

    /* Check the domains of functions and their error handling. */
    ai4->ai_addr->sa_family = AF_UNIX;
    ok(!network_sockaddr_equal(ai4->ai_addr, ai4->ai_addr),
       "equal not equal with address mismatches");
    is_int(0, network_sockaddr_port(ai4->ai_addr),
           "port meaningless for AF_UNIX");
    freeaddrinfo(ai4);

    /* Tests for network_addr_compare. */
    ok_addr(1, "127.0.0.1", "127.0.0.1",   NULL);
    ok_addr(0, "127.0.0.1", "127.0.0.2",   NULL);
    ok_addr(1, "127.0.0.1", "127.0.0.0",   "31");
    ok_addr(0, "127.0.0.1", "127.0.0.0",   "32");
    ok_addr(0, "127.0.0.1", "127.0.0.0",   "255.255.255.255");
    ok_addr(1, "127.0.0.1", "127.0.0.0",   "255.255.255.254");
    ok_addr(1, "10.10.4.5", "10.10.4.255", "24");
    ok_addr(0, "10.10.4.5", "10.10.4.255", "25");
    ok_addr(1, "10.10.4.5", "10.10.4.255", "255.255.255.0");
    ok_addr(0, "10.10.4.5", "10.10.4.255", "255.255.255.128");
    ok_addr(0, "129.0.0.0", "1.0.0.0",     "1");
    ok_addr(1, "129.0.0.0", "1.0.0.0",     "0");
    ok_addr(1, "129.0.0.0", "1.0.0.0",     "0.0.0.0");

    /* Try some IPv6 addresses. */
#ifdef HAVE_INET6
    ok_addr(1, ipv6_addr,   ipv6_addr,     NULL);
    ok_addr(1, ipv6_addr,   ipv6_addr,     "128");
    ok_addr(1, ipv6_addr,   ipv6_addr,     "60");
    ok_addr(1, "::127",     "0:0::127",    "128");
    ok_addr(1, "::127",     "0:0::128",    "120");
    ok_addr(0, "::127",     "0:0::128",    "128");
    ok_addr(0, "::7fff",    "0:0::8000",   "113");
    ok_addr(1, "::7fff",    "0:0::8000",   "112");
    ok_addr(0, "::3:ffff",  "::2:ffff",    "120");
    ok_addr(0, "::3:ffff",  "::2:ffff",    "119");
    ok_addr(0, "ffff::1",   "7fff::1",     "1");
    ok_addr(1, "ffff::1",   "7fff::1",     "0");
    ok_addr(0, "fffg::1",   "fffg::1",     NULL);
    ok_addr(0, "ffff::1",   "7fff::1",     "-1");
    ok_addr(0, "ffff::1",   "ffff::1",     "-1");
    ok_addr(0, "ffff::1",   "ffff::1",     "129");
#else
    skip_block(16, "IPv6 not supported");
#endif

    /* Test some invalid addresses. */
    ok_addr(0, "fred",      "fred",        NULL);
    ok_addr(0, "",          "",            NULL);
    ok_addr(0, "",          "",            "0");
    ok_addr(0, "127.0.0.1", "127.0.0.1",   "pete");
    ok_addr(0, "127.0.0.1", "127.0.0.1",   "1p");
    ok_addr(0, "127.0.0.1", "127.0.0.1",   "1p");
    ok_addr(0, "127.0.0.1", "127.0.0.1",   "-1");
    ok_addr(0, "127.0.0.1", "127.0.0.1",   "33");

    return 0;
}
