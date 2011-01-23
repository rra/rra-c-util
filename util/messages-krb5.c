/*
 * Error handling for Kerberos v5.
 *
 * Provides versions of die and warn that take a Kerberos context and a
 * Kerberos error code and append the Kerberos error message to the provided
 * formatted message.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2006, 2007, 2008, 2009, 2010
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <config.h>
#include <portable/krb5.h>
#include <portable/system.h>

#include <util/macros.h>
#include <util/messages.h>
#include <util/messages-krb5.h>
#include <util/xmalloc.h>


/*
 * Report a Kerberos error and exit.
 */
void
die_krb5(krb5_context ctx, krb5_error_code code, const char *format, ...)
{
    const char *k5_msg = NULL;
    char *message;
    va_list args;

    k5_msg = krb5_get_error_message(ctx, code);
    va_start(args, format);
    if (xvasprintf(&message, format, args) < 0)
        die("internal error: unable to format error message");
    va_end(args);
    die("%s: %s", message, k5_msg);
}


/*
 * Report a Kerberos error.
 */
void
warn_krb5(krb5_context ctx, krb5_error_code code, const char *format, ...)
{
    const char *k5_msg = NULL;
    char *message;
    va_list args;

    k5_msg = krb5_get_error_message(ctx, code);
    va_start(args, format);
    if (xvasprintf(&message, format, args) < 0)
        die("internal error: unable to format error message");
    va_end(args);
    warn("%s: %s", message, k5_msg);
    free(message);
    krb5_free_error_message(ctx, k5_msg);
}
