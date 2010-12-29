/*
 * Constructor and destructor for PAM data.
 *
 * The PAM utility functions often need an initial argument that encapsulates
 * the PAM handle, some configuration information, and possibly a Kerberos
 * context.  This implements a constructor and destructor for that data
 * structure.
 *
 * The individual PAM modules should provide a definition of the pam_config
 * struct appropriate to that module.  None of the PAM utility functions need
 * to know what that configuration struct looks like, and it must be freed
 * before calling putil_args_free().
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2010 Board of Trustees, Leland Stanford Jr. University
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#ifdef HAVE_KERBEROS
# include <portable/krb5.h>
#endif
#include <portable/pam.h>
#include <portable/system.h>

#include <errno.h>

#include <pam-util/args.h>
#include <pam-util/logging.h>


/*
 * Allocate a new pam_args struct and return it, or NULL on memory allocation
 * or Kerberos initialization failure.  If HAVE_KERBEROS is defined, we also
 * allocate a Kerberos context.
 */
struct pam_args *
putil_args_new(pam_handle_t *pamh, int flags)
{
    struct pam_args *args;
#ifdef HAVE_KERBEROS
    krb5_error_code status;
#endif

    args = calloc(1, sizeof(struct pam_args));
    if (args == NULL) {
        putil_crit(NULL, "cannot allocate memory: %s", strerror(errno));
        return NULL;
    }
    args->pamh = pamh;
    args->config = NULL;
    args->user = NULL;
    args->silent = ((flags & PAM_SILENT) == PAM_SILENT);

#ifdef HAVE_KERBEROS
    args->realm = NULL;
    if (issetuidgid())
        status = krb5_init_secure_context(&args->ctx);
    else
        status = krb5_init_context(&args->ctx);
    if (status != 0) {
        putil_err_krb5(args, status, "cannot create Kerberos context");
        free(args);
        return NULL;
    }
#endif /* HAVE_KERBEROS */
    return args;
}


/*
 * Free a pam_args struct.  The config member must be freed separately.
 */
void
putil_args_free(struct pam_args *args)
{
#ifdef HAVE_KERBEROS
    if (args->realm != NULL)
        free(args->realm);
    if (args->ctx != NULL)
        krb5_free_context(args->ctx);
#endif
    free(args);
}
