/*
 * Testing interfaces to the fake PAM library.
 *
 * This header defines the interfaces to the fake PAM library that are used by
 * test code to initialize the library and recover test data from it.  We
 * don't define any interface that we're going to duplicate from the main PAM
 * API.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */
 
#ifndef FAKEPAM_TESTING_H
#define FAKEPAM_TESTING_H 1

#include <config.h>
#include <portable/pam.h>
#include <portable/macros.h>

/* Used inside the fake PAM library to hold data items. */
struct fakepam_data {
    char *name;
    void *data;
    void (*cleanup)(pam_handle_t *, void *, int);
    struct fakepam_data *next;
};

/* This is an opaque data structure, so we can put whatever we want in it. */
struct pam_handle {
    const char *service;
    const char *user;
    const struct pam_conv *conversation;
    const char **environ;
    struct fakepam_data *data;
};

BEGIN_DECLS

/*
 * Returns the accumulated messages logged with pam_syslog or pam_vsyslog
 * since the last call to pam_output and then clears the output.  Returns
 * newly allocated memory that the caller is responsible for freeing, or NULL
 * if no output has been logged since the last call or since startup.
 */
char *pam_output(void);

END_DECLS

#endif /* !FAKEPAM_API_H */
