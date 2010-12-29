/*
 * Vector handling (counted lists of char *'s).
 *
 * A vector is a table for handling a list of strings with less overhead than
 * linked list.  The intention is for vectors, once allocated, to be reused;
 * this saves on memory allocations once the array of char *'s reaches a
 * stable size.
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

#include <config.h>
#include <portable/system.h>

#include <pam-util/vector.h>


/*
 * Allocate a new, empty vector.  Returns NULL if memory allocation fails.
 */
struct vector *
vector_new(void)
{
    struct vector *vector;

    vector = malloc(sizeof(struct vector));
    if (vector == NULL)
        return NULL;
    vector->count = 0;
    vector->allocated = 0;
    vector->strings = NULL;
    return vector;
}


/*
 * Resize a vector (using realloc to resize the table).  Return false if
 * memory allocation fails.
 */
bool
vector_resize(struct vector *vector, size_t size)
{
    size_t i;
    char **strings;

    if (vector->count > size) {
        for (i = size; i < vector->count; i++)
            free(vector->strings[i]);
        vector->count = size;
    }
    if (size == 0) {
        free(vector->strings);
        vector->strings = NULL;
    } else {
        strings = realloc(vector->strings, size * sizeof(char *));
        if (strings == NULL)
            return false;
        vector->strings = strings;
    }
    vector->allocated = size;
    return true;
}


/*
 * Add a new string to the vector, resizing the vector as necessary.  The
 * vector is resized an element at a time; if a lot of resizes are expected,
 * vector_resize should be called explicitly with a more suitable size.
 * Return false if memory allocation fails.
 */
bool
vector_add(struct vector *vector, const char *string)
{
    size_t next = vector->count;

    if (vector->count == vector->allocated)
        if (!vector_resize(vector, vector->allocated + 1))
            return false;
    vector->strings[next] = strdup(string);
    if (vector->strings[next] == NULL)
        return false;
    vector->count++;
    return true;
}


/*
 * Empty a vector but keep the allocated memory for the pointer table.
 */
void
vector_clear(struct vector *vector)
{
    size_t i;

    for (i = 0; i < vector->count; i++)
        free(vector->strings[i]);
    vector->count = 0;
}


/*
 * Free a vector completely.
 */
void
vector_free(struct vector *vector)
{
    vector_clear(vector);
    free(vector->strings);
    free(vector);
}


/*
 * Given a vector that we may be reusing, clear it out.  If the first argument
 * is NULL, allocate a new vector.  Used by vector_split*.  Returns NULL if
 * memory allocation fails.
 */
static struct vector *
vector_reuse(struct vector *vector)
{
    if (vector == NULL)
        return vector_new();
    else {
        vector_clear(vector);
        return vector;
    }
}


/*
 * Given a string and a set of separators expressed as a string, count the
 * number of strings that it will split into when splitting on those
 * separators.
 */
static size_t
split_multi_count(const char *string, const char *seps)
{
    const char *p;
    size_t count;

    if (*string == '\0')
        return 0;
    for (count = 1, p = string + 1; *p != '\0'; p++)
        if (strchr(seps, *p) != NULL && strchr(seps, p[-1]) == NULL)
            count++;

    /*
     * If the string ends in separators, we've overestimated the number of
     * strings by one.
     */
    if (strchr(seps, p[-1]) != NULL)
        count--;
    return count;
}


/*
 * Given a string, split it at any of the provided separators to form a
 * vector, copying each string segment.  If the third argument isn't NULL,
 * reuse that vector; otherwise, allocate a new one.  Any number of
 * consecutive separators are considered a single separator.  Returns NULL on
 * memory allocation failure, after which the provided vector may only have
 * partial results.
 */
struct vector *
vector_split_multi(const char *string, const char *seps,
                   struct vector *vector)
{
    const char *p, *start;
    size_t i, count;
    bool created = false;

    if (vector == NULL)
        created = true;
    vector = vector_reuse(vector);
    if (vector == NULL)
        return NULL;

    count = split_multi_count(string, seps);
    if (vector->allocated < count && !vector_resize(vector, count))
        goto fail;

    vector->count = 0;
    for (start = string, p = string, i = 0; *p; p++)
        if (strchr(seps, *p) != NULL) {
            if (start != p) {
                vector->strings[i] = strndup(start, p - start);
                if (vector->strings[i] == NULL)
                    goto fail;
                i++;
                vector->count++;
            }
            start = p + 1;
        }
    if (start != p) {
        vector->strings[i] = strndup(start, p - start);
        if (vector->strings[i] == NULL)
            goto fail;
        vector->count++;
    }
    return vector;

fail:
    if (created)
        vector_free(vector);
    return NULL;
}
