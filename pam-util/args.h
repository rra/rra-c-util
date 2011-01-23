/*
 * Standard structure for PAM data.
 *
 * The PAM utility functions often need an initial argument that encapsulates
 * the PAM handle, some configuration information, and possibly a Kerberos
 * context.  This header provides a standard structure definition.
 *
 * The individual PAM modules should provide a definition of the pam_config
 * struct appropriate to that module.  None of the PAM utility functions need
 * to know what that configuration struct looks like.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#ifndef PAM_UTIL_ARGS_H
#define PAM_UTIL_ARGS_H 1

#include <config.h>
#ifdef HAVE_KERBEROS
# include <portable/krb5.h>
#endif
#include <portable/pam.h>
#include <portable/stdbool.h>

/* Opaque struct from the PAM utility perspective. */
struct pam_config;

struct pam_args {
    pam_handle_t *pamh;         /* Pointer back to the PAM handle. */
    struct pam_config *config;  /* Per-module PAM configuration. */
    bool debug;                 /* Log debugging information. */
    bool silent;                /* Do not pass text to the application. */
    const char *user;           /* User being authenticated. */

#ifdef HAVE_KERBEROS
    krb5_context ctx;           /* Context for Kerberos operations. */
    char *realm;                /* Kerberos realm for configuration. */
#endif
};

BEGIN_DECLS

/* Default to a hidden visibility for all internal functions. */
#pragma GCC visibility push(hidden)

/*
 * Allocate and free the pam_args struct.  We assume that user is a pointer to
 * a string maintained elsewhere and don't free it here.  config must be freed
 * separately by the caller.
 */
struct pam_args *putil_args_new(pam_handle_t *, int flags);
void putil_args_free(struct pam_args *);

/* Undo default visibility change. */
#pragma GCC visibility pop

END_DECLS

#endif /* !PAM_UTIL_ARGS_H */
