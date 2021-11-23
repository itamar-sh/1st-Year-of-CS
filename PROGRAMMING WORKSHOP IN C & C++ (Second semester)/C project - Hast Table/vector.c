//
// Created by itamar on 15/05/2021.
//
#include "vector.h"
void fix_load_factor_up(vector *vector);
void fix_load_factor_down(vector *vector);

vector *vector_alloc(vector_elem_cpy elem_copy_func,
                     vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
{
    vector *v = malloc(sizeof (vector));
    if (v == NULL){
        return NULL;
    }
    v->size = 0;
    v->capacity = VECTOR_INITIAL_CAP;
    v->data = malloc(sizeof (void*)*VECTOR_INITIAL_CAP);
    v->elem_cmp_func = elem_cmp_func;
    v->elem_copy_func = elem_copy_func;
    v->elem_free_func = elem_free_func;
    return v;
}

void vector_free(vector **p_vector)
{
    if (p_vector == NULL || (*p_vector == NULL)){
        return;
    }
    vector_elem_free func = (*p_vector)->elem_free_func;
    for (size_t i =0;i<(*p_vector)->size;i++){
        func(&((*p_vector)->data[i]));
    }
    free((*p_vector)->data);
    free(*p_vector);
    *p_vector = NULL;
}

void *vector_at(const vector *vector, size_t ind)
{
    if (vector == NULL || vector->size <= ind){
        return NULL;
    }
    return vector->data[ind];
}

int vector_find(const vector *vector, const void *value)
{
    if (vector == NULL || value == NULL){
        return -1;
    }
    for (size_t i = 0;i<vector->size;i++){
        if(vector->elem_cmp_func(vector->data[i], value) == 1){
            return i;
        }
    }
    return -1;
}

int vector_push_back(vector *vector, const void *value)
{
    if (vector == NULL || value == NULL){
        return 0;
    }
    vector->size++;
    vector->data[vector->size-1] = vector->elem_copy_func(value);
    double load_factor = vector_get_load_factor(vector);
    if (load_factor >= VECTOR_MAX_LOAD_FACTOR){
        fix_load_factor_up(vector);
    }
    return 1;
}

// extend capacity of vector
void fix_load_factor_up(vector *vector)
{
    vector->capacity = vector->capacity*VECTOR_GROWTH_FACTOR;
    void **p = realloc(vector->data, sizeof (void*)*vector->capacity);
    if (p == NULL){
        return; //we have a problem here.
    }
    vector->data = p;
}

double vector_get_load_factor(const vector *vector)
{
    if (vector == NULL){
        return -1;
    }
    return (double)vector->size / (double)vector->capacity;
}

int vector_erase(vector *vector, size_t ind)
{
    if (vector == NULL || vector->size <= ind){
        return 0;
    }
    vector_elem_free func = vector->elem_free_func;
    func(&(vector->data[ind]));
    for (int i=0;i+ind+1<vector->size;i++){
        vector->data[ind+i] = vector->data[ind+i+1];
    }
    vector->size--;
    double load_factor = vector_get_load_factor(vector);
    if (load_factor <= VECTOR_MIN_LOAD_FACTOR){
        fix_load_factor_down(vector);
    }
    return 1;
}

// shrink capacity of vector
void fix_load_factor_down(vector *vector)
{
    vector->capacity = vector->capacity*0.5;
    void **p = realloc(vector->data, sizeof (void*)*vector->capacity);
    if (p == NULL){
        return; //we have a problem here.
    }
    vector->data = p;
}

void vector_clear(vector *vector)
{
    for (size_t i=0;i<vector->size;i++){
        //vector_erase(vector, 0);
        vector->data[i] = NULL;
    }
    vector->size = 0;
    void **p = realloc(vector->data, sizeof (void*)*VECTOR_INITIAL_CAP);
    if (p == NULL){
        return; //we have a problem here.
    }
    vector->data = p;
    vector->capacity = VECTOR_INITIAL_CAP;
}