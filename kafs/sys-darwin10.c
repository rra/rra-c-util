/*
 * AFS system call for Mac OS X 10.6 systems (and later).
 *
 * This is an AFS system call implementation for Mac OS X 10.6 systems (and
 * later).  It is for use on systems that don't have libkafs or libkopenafs,
 * or where a dependency on those libraries is not desirable for some reason.
 *
 * This file is included by kafs/kafs.c on Mac OS X 10.6 platforms and
 * therefore doesn't need its own copy of standard includes, only whatever
 * additional data is needed for the Linux interface.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2009, 2010
 *     Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

/*
 * The struct passed to ioctl to do an AFS system call.  Definition taken from
 * the afs/afs_args.h OpenAFS header.  We choose one of two structs depending
 * on whether we have a 32-bit or 64-bit interface.  user_addr_t should be
 * provided by sys/types.h on Mac OS X.
 */
struct afssysargs {
    unsigned int syscall;
    unsigned int param1;
    unsigned int param2;
    unsigned int param3;
    unsigned int param4;
    unsigned int param5;
    unsigned int param6;
    unsigned int retval;
};
struct afssysargs64 {
    user_addr_t param1;
    user_addr_t param2;
    user_addr_t param3;
    user_addr_t param4;
    user_addr_t param5;
    user_addr_t param6;
    unsigned int syscall;
    unsigned int retval;
};


/*
 * The workhorse function that does the actual system call.  All the values
 * are passed as longs to match the internal OpenAFS interface, which means
 * that there's all sorts of ugly type conversion happening here.
 *
 * Returns -1 and sets errno to ENOSYS if attempting a system call fails and 0
 * otherwise.  If the system call was made, its return status will be stored
 * in rval.
 */
static int
k_syscall(long call, long param1, long param2, long param3, long param4,
          int *rval)
{
    int fd, code, oerrno;

    fd = open("/dev/openafs_ioctl", O_RDWR);
    if (fd < 0) {
        errno = ENOSYS;
        return -1;
    }

    if (sizeof(param1) == 8) {
        struct afssysargs64 syscall_data;

        syscall_data.syscall = call;
        syscall_data.param1 = param1;
        syscall_data.param2 = param2;
        syscall_data.param3 = param3;
        syscall_data.param4 = param4;
        syscall_data.param5 = 0;
        syscall_data.param6 = 0;
        code = ioctl(fd, _IOWR('C', 2, struct afssysargs64), &syscall_data);
    } else {
        struct afssysargs syscall_data;

        syscall_data.syscall = call;
        syscall_data.param1 = param1;
        syscall_data.param2 = param2;
        syscall_data.param3 = param3;
        syscall_data.param4 = param4;
        syscall_data.param5 = 0;
        syscall_data.param6 = 0;
        code = ioctl(fd, _IOWR('C', 1, struct afssysargs), &syscall_data);
    }
    if (code == 0)
        *rval = syscall_data.retval;

    oerrno = errno;
    close(fd);
    errno = oerrno;
    return code;
}
