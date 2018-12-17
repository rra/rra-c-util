#!/usr/bin/perl
#
# Build script for an empty module.
#
# We want to run all the Perl test scripts as part of the test process of
# rra-c-util or things like perltidy failures won't be noticed.  That in turn
# requires a build script and a module.  This is a minimum build script, just
# enough to let tests run.
#
# Copyright 2018 Russ Allbery <eagle@eyrie.org>
#
# SPDX-License-Identifier: MIT

use strict;
use warnings;

use Module::Build;

my $build = Module::Build->new(
    dist_abstract        => 'Empty test module',
    dist_author          => 'Russ Allbery <eagle@eyrie.org>',
    license              => 'mit',
    module_name          => 'Empty',
    recursive_test_files => 1,
    add_to_cleanup       => [qw(MANIFEST.bak MYMETA.json.lock cover_db)],
    configure_requires   => { 'Module::Build' => 0.36 },
);
$build->create_build_script;
