#!/usr/bin/perl
#
# Check all POD documents for POD formatting errors.
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

# Skip tests if Test::Pod is not installed.
if (!eval { require Test::Pod }) {
    plan skip_all => 'Test::Pod required to test POD syntax';
}
Test::Pod->import;

# Check all POD in the Perl distribution.  Add the examples directory.
my @files = all_pod_files();
if (-d 'examples') {
    push @files, 'examples';
}
all_pod_files_ok(@files);
