/*
 * Replacement for a missing issetuidgid.
 *
 * Simulates the functionality as the Solaris function issetuidgid, which
 * returns true if the running program was setuid or setgid.  The replacement
 * test is not quite as comprehensive as what the Solaris function does, but
 * it should be good enough.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * This work is hereby placed in the public domain by its author.
 */

#include <config.h>
#include <portable/system.h>

int
issetuidgid(void)
{
    if (getuid() != geteuid())
        return 1;
    if (getgid() != getegid())
        return 1;
    return 0;
}
