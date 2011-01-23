/*
 * Utility functions to test message handling.
 *
 * These functions set up a message handler to trap warn and notice output
 * into a buffer that can be inspected later, allowing testing of error
 * handling.
 *
 * Copyright 2002, 2004, 2005 Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2009
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <tests/tap/messages.h>
#include <util/concat.h>
#include <util/macros.h>
#include <util/messages.h>
#include <util/xmalloc.h>

/* A global buffer into which message_log_buffer stores error messages. */
char *errors = NULL;


/*
 * An error handler that appends all errors to the errors global.  Used by
 * error_capture.
 */
static void
message_log_buffer(int len, const char *fmt, va_list args, int error UNUSED)
{
    char *message;

    message = xmalloc(len + 1);
    vsnprintf(message, len + 1, fmt, args);
    if (errors == NULL) {
        errors = concat(message, "\n", (char *) 0);
    } else {
        char *new_errors;

        new_errors = concat(errors, message, "\n", (char *) 0);
        free(errors);
        errors = new_errors;
    }
    free(message);
}


/*
 * Turn on the capturing of errors.  Errors will be stored in the global
 * errors variable where they can be checked by the test suite.  Capturing is
 * turned off with errors_uncapture.
 */
void
errors_capture(void)
{
    if (errors != NULL) {
        free(errors);
        errors = NULL;
    }
    message_handlers_warn(1, message_log_buffer);
    message_handlers_notice(1, message_log_buffer);
}


/*
 * Turn off the capturing of errors again.
 */
void
errors_uncapture(void)
{
    message_handlers_warn(1, message_log_stderr);
    message_handlers_notice(1, message_log_stdout);
}
