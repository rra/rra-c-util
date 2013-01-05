#!/usr/bin/perl
#
# Check for spelling errors in POD documentation.
#
# Checks all POD files in a Perl distribution using Test::Spelling.  This test
# is disabled unless RRA_MAINTAINER_TESTS is set, since spelling dictionaries
# vary too much between environments.
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

# Skip tests unless we're running the test suite in maintainer mode.
if (!$ENV{RRA_MAINTAINER_TESTS}) {
    plan skip_all => 'Spelling tests only run for maintainer';
}

# Skip tests if Test::Spelling is not installed.
if (!eval { require Test::Spelling }) {
    plan skip_all => 'Test::Spelling required to test POD spelling';
}
Test::Spelling->import;

# Check all POD in the Perl distribution.  Add the examples directory.
my @files = all_pod_files();
push @files, 'examples';
all_pod_files_spelling_ok(@files);
