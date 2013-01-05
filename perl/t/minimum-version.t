#!/usr/bin/perl
#
# Check that too-new features of Perl are not being used.
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

# Minimum version of Perl required.
my $MINIMUM_VERSION = '5.006';

# Skip tests if Test::MinimumVersion is not installed.
if (!eval { require Test::MinimumVersion }) {
    plan skip_all => 'Test::MinimumVersion required to test version limits';
}
Test::MinimumVersion->import;

# Check most files in the Perl distribution.  Note that this doesn't check
# examples or any other random directories with scripts, and there isn't any
# simple way to add more directories.
all_minimum_version_ok($MINIMUM_VERSION);
