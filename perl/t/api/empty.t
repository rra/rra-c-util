#!/usr/bin/perl
#
# Tiny test solely to get coverage information and let tests pass.
#
# Copyright 2018-2019, 2021, 2024 Russ Allbery <eagle@eyrie.org>
#
# SPDX-License-Identifier: MIT

use 5.012;
use warnings;

use lib 't/lib';

use Test::RRA;

use Test::More tests => 2;

# Load the module.
BEGIN { use_ok('Empty') }

# Test the function.
is(Empty::empty_function(), 42, 'test_function');
