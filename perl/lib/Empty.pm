# Empty module for test purposes.
#
# This module exists only to trigger the test suite, with just enough code to
# get some interesting tests to run and my standard module boilerplate in case
# any tests expect it.
#
# SPDX-License-Identifier: MIT

package Empty;

use 5.006;
use strict;
use warnings;

# Declare variables that should be set in BEGIN for robustness.
our $VERSION;

# Set $VERSION and everything export-related in a BEGIN block for robustness
# against circular module loading (not that we load any modules, but
# consistency is good).
BEGIN {
    $VERSION = '8.00';
}

# Empty function for testing purposes.
sub empty_function {
    return 42;
}

1;
__END__

=for stopwords
Allbery rra-c-util MERCHANTABILITY NONINFRINGEMENT sublicense

=head1 NAME

Empty - Empty module for test purposes

=head1 SYNOPSIS

    use Empty;

=head1 DESCRIPTION

An empty module that does nothing, used only for test and example purposes.
It's intended to be just enough of a Perl module to trigger meaningful tests.

=head1 FUNCTIONS

=over 4

=item empty_function

Returns 42.

=back

=head1 AUTHOR

Russ Allbery <eagle@eyrie.org>

=head1 COPYRIGHT AND LICENSE

Copyright 2018 Russ Allbery <eagle@eyrie.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

=head1 SEE ALSO

This module is maintained in the rra-c-util package.  The current version is
available from L<https://www.eyrie.org/~eagle/software/rra-c-util/>.

=cut

# Local Variables:
# copyright-at-end-flag: t
# End:
