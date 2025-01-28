//
// Created by itamar on 20/05/2021.
//

#include "test_suite.h"
#include "hash_funcs.h"
#include <stdio.h>
//checks
int check_insert_char();
int check_insert_int();
int is_digit (const_keyT elem);
void double_value (valueT elem);


// pairs making
void *char_key_cpy (const_keyT key);
int char_key_cmp (const_keyT key_1, const_keyT key_2);
void char_key_free (keyT* key);
void *int_key_cpy (const_keyT key);
int int_key_cmp (const_keyT key_1, const_keyT key_2);
void int_key_free (keyT* key);
void *int_value_cpy (const_valueT value);
int int_value_cmp (const_valueT val_1, const_valueT val_2);
void int_value_free (valueT *val);
void *double_value_cpy (const_valueT value);
int double_value_cmp (const_valueT val_1, const_valueT val_2);
void double_value_free (valueT *val);

void test_hash_map_insert(void)
{
    assert(check_insert_char() == 0);
    assert(check_insert_int() == 0);
}

void test_hash_map_at(void)
{
    // char
    char key;
    int value;
    pair *pairs[128];
    for (int j = 0; j < 128; ++j)
    {
        key = (char) (j);
        value = j;
        pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                               int_value_cmp, char_key_free, int_value_free);
    }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_bad);
    for (int k = 0; k < 128; ++k)
    {
        hashmap_insert (map, pairs[k]);
        key = (char) (k);
        int num = *((int *)hashmap_at (map, &key));
        assert(num == k); // check exist value
    }
    // check not exist value
    key = (char) (150);
    assert(hashmap_at (map, &key) == NULL);
    // free
    for (int k_i = 0; k_i < 128; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&map);
    return;
}

void test_hash_map_erase(void)
{
    char key;
    int value;
    pair *pairs[128];
    for (int j = 0; j < 128; ++j)
    {
        key = (char) (j);
        value = j;
        pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                               int_value_cmp, char_key_free, int_value_free);
    }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_bad);
    for (int k = 0; k < 128; ++k)
    {
        hashmap_insert (map, pairs[k]);
    }
    for(int k = 0; k < 128; ++k)
    {
        key = (char) (k);
        hashmap_erase(map, &key);
        assert(hashmap_at(map, &key) == NULL);
        assert(map->size == (unsigned int)(128-k-1));
    }
    assert(map->capacity == 2);
    // free
    for (int k_i = 0; k_i < 128; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&map);
    //
    return;
}

void test_hash_map_get_load_factor(void)
{
    char key;
    int value;
    pair *pairs[128];
    for (int j = 0; j < 128; ++j)
    {
        key = (char) (j);
        value = j;
        pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                               int_value_cmp, char_key_free, int_value_free);
    }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_bad);
    for (int k = 0; k < 128; ++k)
    {
        hashmap_insert (map, pairs[k]);
    }
    for(int k = 0; k < 128; ++k)
    {
        assert(hashmap_get_load_factor(map) <= 0.75 && hashmap_get_load_factor(map) >= 0.25);
        key = (char) (k);
        hashmap_erase(map, &key);
    }
//    for (int j = 0; j < 128; ++j)
//    {
//        free(pairs[j]);
//    }
//    //free(map->buckets);
//    hashmap_free (&map);

    for (int k_i = 0; k_i < 128; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&map);
}

void test_hash_map_apply_if()
{
    // Create Pairs.
    pair *pairs[16];
    for (int j = 0; j < 16; ++j)
    {
     char key = (char) (j + 48);
     //even keys are capital letters, odd keys are digits
     if (key % 2)
     {
        key += 17;
     }
     int value = j;
     pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
     int_value_cmp, char_key_free, int_value_free);
    }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_char);
    for (int k = 0; k < 16; ++k)
    {
    hashmap_insert (map, pairs[k]);
    }
    //apply double_value on values where key is a digit
    //char key_dig = '2', key_letter = 'D';
    //int apply_if_res = hashmap_apply_if (map, is_digit, double_value);
    //assert(apply_if_res == 4);
    // Free the pairs.
    // Free the hash-map.
    for (int k_i = 0; k_i < 16; ++k_i)
    {
    pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&map);

}

int check_insert_char()
{
    hashmap *h_char = hashmap_alloc(hash_char);
    // single check
    char key = 'A';
    int value = 5;
    pair* p = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                          int_value_cmp, char_key_free, int_value_free);
    assert (hashmap_insert(h_char, p) == 1);
    size_t ind = h_char->hash_func(&key) & (h_char->capacity -1);
    if (*((char *)(((pair *)(h_char->buckets[ind])->data[0])->value)) != value){
        return 1; // not insert the right value;
    }
    // make pairs
    pair *pairs[400];
    for (int j = 0; j < 400; ++j)
    {
        key = (char) (j);
        value = j;
        pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                               int_value_cmp, char_key_free, int_value_free);
    }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_char);
    for (int k = 0; k < 20; ++k)
    {
        hashmap_insert (map, pairs[k]);
        assert(map->size == (size_t)(k+1));
    }
    assert (map->capacity == 32);
    assert (map->size == 20);
    for (int k = 20; k < 400; ++k)
    {
        hashmap_insert (map, pairs[k]);
    }
    assert (map->capacity == 512);
    assert (map->size == 256);
    // check if bad hash is working
    hashmap *map_bad = hashmap_alloc (hash_bad);
    for (int k = 0; k < 400; ++k)
    {
        hashmap_insert (map_bad, pairs[k]);
    }
    assert(map_bad->buckets[0]->size == 256); // everything is under the same vector
    // free part
    pair_free((void **)&p);
    for (int k_i = 0; k_i < 400; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&h_char);
    hashmap_free (&map);
    hashmap_free (&map_bad);
    return 0;
}

int check_insert_int()
{
    hashmap *h_int = hashmap_alloc(hash_int);
    // single check
    int key = 65;
    double value = 5.0;
    pair* p = pair_alloc (&key, &value, int_key_cpy, double_value_cpy, int_key_cmp,
                          double_value_cmp, int_key_free, double_value_free);
    assert (hashmap_insert(h_int, p) == 1);
    size_t ind = h_int->hash_func(&key) & (h_int->capacity -1);
    if (*((double *)(((pair *)(h_int->buckets[ind])->data[0])->value)) != value){
            return 1; // not insert the right value;
        }
    // make pairs
    pair *pairs[400];
    for (int j = 0; j < 400; ++j)
        {
            key = j;
            value = j+0.5;
            pairs[j] = pair_alloc (&key, &value, int_key_cpy, double_value_cpy, int_key_cmp,
                                             double_value_cmp, int_key_free,
                                             double_value_free);
        }
    // Create hash-map and inserts elements into it, using pair_char_int.h
    hashmap *map = hashmap_alloc (hash_int);
    for (int k = 0; k < 20; ++k)
    {
        hashmap_insert (map, pairs[k]);
        assert(map->size == (size_t)k+1);
    }
    assert (map->capacity == 32);
    assert (map->size == 20);
    for (int k = 20; k < 400; ++k)
    {
        hashmap_insert (map, pairs[k]);
    }
    assert (map->capacity == 1024);
    assert (map->size == 400);
    // check if bad hash is working
    hashmap *map_bad = hashmap_alloc (hash_bad);
    for (int k = 0; k < 400; ++k)
    {
        hashmap_insert (map_bad, pairs[k]);
        assert(*((double *)((pair *)(map_bad->buckets[0]->data[k]))->value) == k+0.5);
    }
    assert(map_bad->buckets[0]->size == 400); // everything is under the same vector
    // free part
    pair_free((void **)&p);
    for (int k_i = 0; k_i < 400; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
    hashmap_free (&h_int);
    hashmap_free (&map);
    hashmap_free (&map_bad);
    return 0;
}

//*** !!!!! ***//
//*** PAIRS ***//
//*** !!!!! ***//

// char key
void *char_key_cpy (const_keyT key)
{
    char *new_char = malloc (sizeof (char));
    *new_char = *((char *) key);
    return new_char;
}

int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
    return *(char *) key_1 == *(char *) key_2;
}

void char_key_free (keyT* key)
{
    if (key && *key)
        {
            free (*key);
            *key = NULL;
        }
}

// char value

void *char_value_cpy (const_valueT value)
{
    char *new_int = malloc (sizeof (char));
    *new_int = *((char *) value);
    return new_int;
}

int char_value_cmp (const_valueT val_1, const_valueT val_2)
{
    return *(char *) val_1 == *(char *) val_2;
}

void char_value_free (valueT *val)
{
    if (val && *val)
        {
            free (*val);
            *val = NULL;
        }
}
// int key

void *int_key_cpy (const_keyT key)
{
    int *new_char = malloc (sizeof (int));
    *new_char = *((int *) key);
    return new_char;
}

int int_key_cmp (const_keyT key_1, const_keyT key_2)
{
    return *(int *) key_1 == *(int *) key_2;
}

void int_key_free (keyT* key)
{
    if (key && *key)
        {
            free (*key);
            *key = NULL;
        }
}

// int value

void *int_value_cpy (const_valueT value)
{
    int *new_int = malloc (sizeof (int));
    *new_int = *((int *) value);
    return new_int;
}

int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
    return *(int *) val_1 == *(int *) val_2;
}

void int_value_free (valueT *val)
{
    if (val && *val)
        {
            free (*val);
            *val = NULL;
        }
}

// double value

// int value

void *double_value_cpy (const_valueT value)
{
    double *new_int = malloc (sizeof (double));
    *new_int = *((double *) value);
    return new_int;
}

int double_value_cmp (const_valueT val_1, const_valueT val_2)
{
    return *(double *) val_1 == *(double *) val_2;
}

void double_value_free (valueT *val)
{
    if (val && *val)
        {
            free (*val);
            *val = NULL;
        }
}


// suffix
/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
    char c = *((char *) elem);
    return (c > 47 && c < 58);
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
    *((int *) elem) *= 2;
}