/*
 * PAM utility vector library test suite.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2009, 2010 Board of Trustees, Leland Stanford Jr. University
 * Copyright (c) 2004, 2005, 2006
 *     by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *     2002, 2003 by The Internet Software Consortium and Rich Salz
 *
 * See LICENSE for licensing terms.
 */

#include <config.h>
#include <portable/system.h>

#include <sys/wait.h>

#include <pam-util/vector.h>
#include <tests/tap/basic.h>


int
main(void)
{
    struct vector *vector, *ovector;
    const char cstring[] = "This is a\ttest.  ";
    char *string;

    plan(41);

    vector = vector_new();
    ok(vector != NULL, "vector_new returns non-NULL");
    ok(vector_add(vector, cstring), "vector_add succeeds");
    is_int(1, vector->count, "vector_add increases count");
    ok(vector->strings[0] != cstring, "...and allocated new memory");
    ok(vector_resize(vector, 4), "vector_resize succeeds");
    is_int(4, vector->allocated, "vector_resize works");
    ok(vector_add(vector, cstring), "vector_add #2");
    ok(vector_add(vector, cstring), "vector_add #3");
    ok(vector_add(vector, cstring), "vector_add #4");
    is_int(4, vector->allocated, "...and no reallocation when adding strings");
    is_int(4, vector->count, "...and the count matches");
    is_string(cstring, vector->strings[0], "added the right string");
    is_string(cstring, vector->strings[1], "added the right string");
    is_string(cstring, vector->strings[2], "added the right string");
    is_string(cstring, vector->strings[3], "added the right string");
    ok(vector->strings[1] != vector->strings[2], "each pointer is different");
    ok(vector->strings[2] != vector->strings[3], "each pointer is different");
    ok(vector->strings[3] != vector->strings[0], "each pointer is different");
    ok(vector->strings[0] != cstring, "each pointer is different");
    vector_clear(vector);
    is_int(0, vector->count, "vector_clear works");
    is_int(4, vector->allocated, "...but doesn't free the allocation");
    string = strdup(cstring);
    if (string == NULL)
        sysbail("cannot allocate memory");
    ok(vector_add(vector, cstring), "vector_add succeeds");
    ok(vector_add(vector, string), "vector_add succeeds");
    is_int(2, vector->count, "added two strings to the vector");
    ok(vector->strings[1] != string, "...and the pointers are different");
    ok(vector_resize(vector, 1), "vector_resize succeeds");
    is_int(1, vector->count, "vector_resize shrinks the vector");
    ok(vector->strings[0] != cstring, "...and the pointer is different");
    vector_free(vector);
    free(string);

    vector = vector_split_multi("foo, bar, baz", ", ", NULL);
    ok(vector != NULL, "vector_split_multi returns non-NULL");
    is_int(3, vector->count, "vector_split_multi returns right count");
    is_string("foo", vector->strings[0], "...first string");
    is_string("bar", vector->strings[1], "...second string");
    is_string("baz", vector->strings[2], "...third string");
    ovector = vector;
    vector = vector_split_multi("", ", ", vector);
    ok(vector != NULL, "reuse of vector doesn't return NULL");
    ok(vector == ovector, "...and reuses the same vector pointer");
    is_int(0, vector->count, "vector_split_multi reuse with empty string");
    is_int(3, vector->allocated, "...and doesn't free allocation");
    vector = vector_split_multi(",,,  foo,   ", ", ", vector);
    ok(vector != NULL, "reuse of vector doesn't return NULL");
    is_int(1, vector->count, "vector_split_multi with extra separators");
    is_string("foo", vector->strings[0], "...first string");
    vector = vector_split_multi(", ,  ", ", ", vector);
    is_int(0, vector->count, "vector_split_multi with only separators");
    vector_free(vector);

    return 0;
}
