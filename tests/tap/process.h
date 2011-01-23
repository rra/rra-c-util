/*
 * Utility functions for tests that use subprocesses.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2009, 2010
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#ifndef TAP_PROCESS_H
#define TAP_PROCESS_H 1

#include <config.h>
#include <portable/macros.h>

BEGIN_DECLS

/*
 * Run a function in a subprocess and check the exit status and expected
 * output (stdout and stderr combined) against the provided values.  Expects
 * the function to always exit (not die from a signal).
 *
 * This reports as three separate tests: whether the function exited rather
 * than was killed, whether the exit status was correct, and whether the
 * output was correct.
 */
typedef void (*test_function_type)(void);
void is_function_output(test_function_type, int status, const char *output,
                        const char *format, ...)
    __attribute__((__format__(printf, 4, 5)));

END_DECLS

#endif /* TAP_PROCESS_H */
