/*
 * Prototypes for vector handling.
 *
 * This is based on the util/vector.c library, but that library uses xmalloc
 * routines to exit the program if memory allocation fails.  This is a
 * modified version of the vector library that instead returns false on
 * failure to allocate memory, allowing the caller to do appropriate recovery.
 *
 * Only the portions of the vector library used by PAM modules are
 * implemented.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * This work is hereby placed in the public domain by its author.
 */

#ifndef PAM_UTIL_VECTOR_H
#define PAM_UTIL_VECTOR_H 1

#include <config.h>
#include <portable/macros.h>
#include <portable/stdbool.h>

#include <sys/types.h>

struct vector {
    size_t count;
    size_t allocated;
    char **strings;
};

BEGIN_DECLS

/* Default to a hidden visibility for all util functions. */
#pragma GCC visibility push(hidden)

/* Create a new, empty vector.  Returns NULL on memory allocation failure. */
struct vector *vector_new(void)
    __attribute__((__malloc__));

/*
 * Add a string to a vector.  Resizes the vector if necessary.  Returns false
 * on failure to allocate memory.
 */
bool vector_add(struct vector *, const char *string)
    __attribute((__nonnull__));

/*
 * Resize the array of strings to hold size entries.  Saves reallocation work
 * in vector_add if it's known in advance how many entries there will be.
 * Returns false on failure to allocate memory.
 */
bool vector_resize(struct vector *, size_t size)
    __attribute((__nonnull__));

/*
 * Reset the number of elements to zero, freeing all of the strings for a
 * regular vector, but not freeing the strings array (to cut down on memory
 * allocations if the vector will be reused).
 */
void vector_clear(struct vector *)
    __attribute((__nonnull__));

/* Free the vector and all resources allocated for it. */
void vector_free(struct vector *)
    __attribute((__nonnull__));

/*
 * Split functions build a vector from a string.  vector_split_multi splits on
 * a set of characters.  If the vector argument is NULL, a new vector is
 * allocated; otherwise, the provided one is reused.  Returns NULL on memory
 * allocation failure, after which the provided vector may have been modified
 * to only have partial results.
 *
 * Empty strings will yield zero-length vectors.  Adjacent delimiters are
 * treated as a single delimiter by vector_split_multi.
 */
struct vector *vector_split_multi(const char *string, const char *seps,
                                  struct vector *)
    __attribute((__nonnull__(1, 2)));

/* Undo default visibility change. */
#pragma GCC visibility pop

END_DECLS

#endif /* UTIL_VECTOR_H */
