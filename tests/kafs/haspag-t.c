/*
 * Test suite for k_haspag.
 *
 * Currently, we can't do a lot to test this, since there's no way to drop a
 * PAG once one is in one.  The average user will be running this in a PAG, so
 * k_haspag will always return true.  But we can at least confirm that.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/kafs.h>
#include <portable/system.h>

#include <tests/tap/basic.h>


int
main(void)
{
    if (!k_hasafs())
        skip_all("AFS not available");

    plan(2);

    is_int(0, k_setpag(), "k_setpag succeeds");
    is_int(1, k_haspag(), "k_haspag now returns true");

    return 0;
}
