#!/usr/bin/perl
#
# Check the SYNOPSIS section of the documentation for syntax errors.
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

# Skip tests if Test::Strict is not installed.
if (!eval { require Test::Synopsis }) {
    plan skip_all => 'Test::Synopsis required to test SYNOPSIS syntax';
}
if (!eval { require Perl::Critic::Utils }) {
    plan skip_all => 'Perl::Critic::Utils required to test SYNOPSIS syntax';
}
Test::Synopsis->import;

# The default Test::Synopsis all_synopsis_ok() function requires that the
# module be in a lib directory.  Use Perl::Critic::Utils to find the modules
# in blib, or lib if it doesn't exist.
my @files = Perl::Critic::Utils::all_perl_files('blib/lib', 'blib/arch');
if (!@files) {
    @files = Perl::Critic::Utils::all_perl_files('lib');
}
plan tests => scalar @files;

# Run the actual tests.
for my $file (@files) {
    synopsis_ok($file);
}
