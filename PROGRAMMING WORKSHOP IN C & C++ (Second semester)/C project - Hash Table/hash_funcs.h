#ifndef HASHFUNCS_H_
#define HASHFUNCS_H_

#include <stdlib.h>

/**
 * Integers simple hash func.
 */
size_t hash_int(const void *elem){
    size_t hash = *((int *) elem);
    return hash;
}

/**
 * Chars simple hash func.
 */
size_t hash_char(const void *elem){
    size_t hash = *((char *) elem);
    return hash;
}

/**
 * Doubles simple hash func.
 */
size_t hash_double(const void *elem){
    size_t hash = *((double *) elem);
    return hash;
}

size_t hash_bad(const void *elem){
    if(elem == NULL){
        return 1;
    }
    return 0;
}

#endif // HASHFUNCS_H_