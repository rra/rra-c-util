#!/usr/bin/perl
#
# Check Changes file for compliance with CPAN::Changes::Spec.
#
# The canonical version of this file is maintained in the rra-c-util package,
# which can be found at <https://www.eyrie.org/~eagle/software/rra-c-util/>.
#
# Copyright 2021, 2024 Russ Allbery <eagle@eyrie.org>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
# SPDX-License-Identifier: MIT

use 5.012;
use warnings;

use lib 't/lib';

use Test::RRA qw(skip_unless_automated use_prereq);

use Test::More;

# Skip this test for normal user installs.
skip_unless_automated('Changes format tests');

# Load prerequisite modules.
use_prereq('Test::CPAN::Changes');

# Run the tests.
changes_ok();
