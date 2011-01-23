/*
 * Utility functions to test message handling.
 *
 * Copyright 2002 Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2009
 *     Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#ifndef TAP_MESSAGES_H
#define TAP_MESSAGES_H 1

#include <config.h>
#include <portable/macros.h>

/* A global buffer into which errors_capture stores errors. */
extern char *errors;

BEGIN_DECLS

/*
 * Turn on capturing of errors with errors_capture.  Errors reported by warn
 * will be stored in the global errors variable.  Turn this off again with
 * errors_uncapture.  Caller is responsible for freeing errors when done.
 */
void errors_capture(void);
void errors_uncapture(void);

END_DECLS

#endif /* !TAP_MESSAGES_H */
