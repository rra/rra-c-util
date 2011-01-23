/*
 * AFS system call via syscall.
 *
 * This is an AFS system call implementation for systems that use syscall,
 * such as Solaris prior to Solaris 11.  It is for use on systems that don't
 * have libkafs or libkopenafs, or where a dependency on those libraries is
 * not desirable for some reason.
 *
 * This file is included by kafs/kafs.c on platforms that use syscall and
 * therefore doesn't need its own copy of standard includes, only whatever
 * additional data is needed for the Linux interface.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2009
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

#ifdef HAVE_AFS_PARAM_H
# include <afs/param.h>
#endif
#include <sys/syscall.h>

int
k_syscall(long call, long param1, long param2, long param3, long param4,
          int *rval)
{
    *rval = syscall(AFS_SYSCALL, call, param1, param2, param3, param4);
    return 0;
}
