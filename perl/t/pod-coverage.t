#!/usr/bin/perl
#
# Test that all methods are documented in POD.
#
# Written by Russ Allbery <rra@stanford.edu>
#
# The authors hereby relinquish any claim to any copyright that they may have
# in this work, whether granted under contract or by operation of law or
# international treaty, and hereby commit to the public, at large, that they
# shall not, at any time in the future, seek to enforce any copyright in this
# work against any person or entity, or prevent any person or entity from
# copying, publishing, distributing or creating derivative works of this work.

use strict;
use warnings;

use Test::More;

# Additional regexes for subs that don't require POD documentation.
my @NO_POD_NEEDED = ();

# Skip tests if Test::Pod::Coverage is not installed.
if (!eval { require Test::Pod::Coverage }) {
    plan skip_all => 'Test::Pod::Coverage required to test POD coverage';
}
Test::Pod::Coverage->import;

# Test everything found in the distribution.
all_pod_coverage_ok({ also_private => [@NO_POD_NEEDED] });
