/*
 * Portability wrapper around <sys/statvfs.h>.
 *
 * Attempts to adjust for systems that have only statfs and not statvfs.  This
 * code is known to not be portable to ULTRIX, which used a completely
 * different struct layout for statfs than other platforms.
 *
 * Be cautious when using this header, since it may define several identifiers
 * (statvfs, f_frsize, f_favail) to fix struct mismatch problems.  Due to how
 * the C preprocessor works, this will affect all uses of those identifiers.
 *
 * The canonical version of this file is maintained in the rra-c-util package,
 * which can be found at <https://www.eyrie.org/~eagle/software/rra-c-util/>.
 *
 * Written by Russ Allbery <eagle@eyrie.org>
 * Copyright 2012
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * Copying and distribution of this file, with or without modification, are
 * permitted in any medium without royalty provided the copyright notice and
 * this notice are preserved.  This file is offered as-is, without any
 * warranty.
 *
 * SPDX-License-Identifier: FSFAP
 */

#ifndef PORTABLE_STATVFS_H
#define PORTABLE_STATVFS_H 1

/*
 * Allow inclusion of config.h to be skipped, since sometimes we have to use a
 * stripped-down version of config.h with a different name.
 */
#ifndef CONFIG_H_INCLUDED
# include <config.h>
#endif

#ifdef HAVE_SYS_STATVFS_H
# include <sys/statvfs.h>
#elif HAVE_SYS_VFS_H
# include <sys/vfs.h>
#endif

/*
 * Rename statvfs to statfs if we don't have statvfs.  This intentionally
 * isn't limited to the function, since we want to rename the structure as
 * well.  Also map some of the more-specific statvfs struct members to the
 * more-general, fallback statfs struct member names.
 */
#ifndef HAVE_STATVFS
# define statvfs  statfs
# define f_frsize f_bsize
# define f_bavail f_bfree
# define f_favail f_ffree
#endif

#endif /* !PORTABLE_STATVFS_H */
