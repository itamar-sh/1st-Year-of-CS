//
// Created by itamar on 16/05/2021.
//
#include <stdio.h>
#include "hashmap.h"

int fix_load_factor_up_h(hashmap *hash_map);
void fix_load_factor_down_h(hashmap *hash_map);
void store_hashmap(vector **buckets, void **arr, int len, int map_size);
void restore_hashmap(hashmap *hash_map, void **arr, int len);

hashmap *hashmap_alloc (hash_func func)
{
    hashmap* h = malloc(sizeof (hashmap)*HASH_MAP_INITIAL_CAP);
    if (h == NULL){
        return NULL;
    }
    h->capacity = HASH_MAP_INITIAL_CAP;
    h->size = 0;
    h->buckets = malloc(sizeof (vector*)*HASH_MAP_INITIAL_CAP);
    if (h->buckets == NULL){
        return NULL;
    } else {
        for (size_t i = 0; i<h->capacity;i++){
            h->buckets[i] = NULL; //make sure everything is null when empty
        }
    }
    h->hash_func = func;
    return h;
}

void hashmap_free (hashmap **p_hash_map)
{
    if (p_hash_map == NULL || (*p_hash_map) == NULL){
        return;
    }
    for (size_t i=0;i<(*p_hash_map)->capacity;i++){
        vector_free(&((*p_hash_map)->buckets[i]));
    }
    free((*p_hash_map)->buckets);
    free(*p_hash_map);
    *p_hash_map = NULL;
}

int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
    if (hashmap_at(hash_map, in_pair->key) != NULL){ // valid check
        return 0;
    }
    size_t ind_before_mod = (hash_map->hash_func)(in_pair->key); // find index
    size_t ind = ind_before_mod & (hash_map->capacity-1);
    if (hash_map->buckets[ind] == NULL){
        hash_map->buckets[ind] = vector_alloc(pair_copy, pair_cmp, pair_free);
    }
    if (vector_find(hash_map->buckets[ind], in_pair)!=-1){
        return 0;
    }
    //insert pair
    int success =vector_push_back(hash_map->buckets[ind], in_pair);
    hash_map->size += success;
    // extend hashmap if needed
    double load_factor = hashmap_get_load_factor(hash_map);
    if (load_factor > HASH_MAP_MAX_LOAD_FACTOR){
        if(fix_load_factor_up_h(hash_map)){
            hashmap_erase(hash_map, in_pair->key); // extend fail-->delete item
        }
    }
    return success;
}

// extend hashmap
int fix_load_factor_up_h(hashmap *hash_map)
{
    if(hash_map == NULL){
        return 1;
    }
    int old_len = hash_map->size;
    void **arr = malloc(sizeof (void*)*old_len);
    // keep items in array and delete the hashmap
    store_hashmap(hash_map->buckets, arr, old_len, hash_map->capacity);
    hash_map->capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
    vector** p = realloc(hash_map->buckets, sizeof (vector*)*hash_map->capacity);
    if (p == NULL){
        return 1; //we have a problem here.
    }
    for (size_t i = 0; i<hash_map->capacity;i++){
        p[i] = NULL;
    }
    hash_map->buckets = p;
    hash_map->size = 0;
    // bring back the items
    restore_hashmap(hash_map, arr, old_len);
    free(arr);
    return 0;
}

valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
    unsigned int ind_vector = (hash_map->hash_func)(key) & (hash_map->capacity-1);
    if (hash_map->buckets[ind_vector] == NULL){
        return NULL;
    }
    for (size_t i = 0; i<hash_map->buckets[ind_vector]->size; i++){
pair_key_cmp key_cmp_func = ((pair *)(hash_map->buckets[ind_vector]->data[i]))->key_cmp;
        keyT cur_key = ((pair *)(hash_map->buckets[ind_vector]->data[i]))->key;
        if(key_cmp_func(key, cur_key) == 1){
            return ((pair *)(hash_map->buckets[ind_vector]->data[i]))->value;
        }
    }
    return NULL;
}

int hashmap_erase (hashmap *hash_map, const_keyT key)
{
    if (hash_map == NULL){
        return 0;
    }
    unsigned int ind_vector = (hash_map->hash_func)(key) & (hash_map->capacity-1);
    if (ind_vector >= hash_map->size){
        return 0;
    }
    unsigned int ind_key = (unsigned int)hash_map->capacity;
    for (unsigned int i = 0; i<hash_map->buckets[ind_vector]->size; i++){
        pair_key_cmp key_cmp_func = ((pair *)(hash_map->buckets[ind_vector]->data[i]))->key_cmp;
        keyT cur_key = ((pair *)(hash_map->buckets[ind_vector]->data[i]))->key;
        if(key_cmp_func(key, cur_key) == 1){
            ind_key = i;
            break;
        }
    }
    if (ind_key == (unsigned int)hash_map->capacity){
        return 0;
    }
    int success = vector_erase(hash_map->buckets[ind_vector], ind_key);
    if (success == 0){
        return 0;
    }
    hash_map->size--;
    double load_factor = hashmap_get_load_factor(hash_map);
    if (load_factor < HASH_MAP_MIN_LOAD_FACTOR){
        fix_load_factor_down_h(hash_map);
    }
    return success;
}

double hashmap_get_load_factor (const hashmap *hash_map)
{
    if (hash_map == NULL){
        return -1;
    }
    return (double)hash_map->size / (double)hash_map->capacity;
}

void fix_load_factor_down_h(hashmap *hash_map)
{
    if(hash_map->size == 0){
        hash_map->capacity = HASH_MAP_GROWTH_FACTOR;
        return;
    }
    if (hash_map == NULL){
        return;
    }
    int old_len = hash_map->size;
    void **arr = malloc(sizeof (void*)*old_len);
    store_hashmap(hash_map->buckets, arr, old_len, hash_map->capacity);
    hash_map->capacity = hash_map->capacity*((float) 1/HASH_MAP_GROWTH_FACTOR);
    vector** p = realloc(hash_map->buckets, sizeof (vector*)*hash_map->capacity);
    if (p == NULL){
        return; //we have a problem here.
    }
    for (size_t i = 0; i<hash_map->capacity;i++){
        p[i] = NULL;
    }
    hash_map->buckets = p;
    hash_map->size = 0;
    restore_hashmap(hash_map, arr, old_len);
    free(arr);
}

int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func, valueT_func valT_func)
{
    int counter =0;
    for (size_t i =0 ; i<hash_map->capacity;i++){
        if (hash_map->buckets[i] == NULL){
            continue;
        }
        int size_of_vector = (hash_map->buckets[i])->size;
        for (int j=0;j<size_of_vector;j++){
            pair * p = (pair *)((hash_map->buckets[i])->data[j]);
            int condition = keyT_func(p->key);
            if (condition){
                valT_func(((pair *)((hash_map->buckets[i])->data[j]))->value);
                counter++;
            }
        }
    }
    return counter;
}

// keep items in map in array
void store_hashmap(vector **buckets, void **arr, int len, int map_size)
{
    int counter = 0;
    for (int i = 0; i<map_size && counter!=len;i++){
        if(buckets[i] == NULL){
            continue;
        }
        for (size_t j=0;j<buckets[i]->size;j++){
            arr[counter] = vector_at(buckets[i], j);
            counter++;
        }
        vector_clear(buckets[i]);
        free(buckets[i]->data);
        free(buckets[i]);
    }
}

// insert items in array to map
void restore_hashmap(hashmap *hash_map, void **arr, int len)
{
    for (int i = 0; i<len;i++){
        hashmap_insert (hash_map, (const pair *)arr[i]);
        pair_free(&arr[i]);
    }
}
