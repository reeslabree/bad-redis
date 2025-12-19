#include "../include/kv.h"

kv_t* kv_create(const char* key, const value_t* val)
{
    // allocate space for the kv pair
    kv_t* pair = malloc(sizeof(kv_t));
    if (!pair)
        return NULL;

    // copy the key onto the kv pair
    pair->key = strdup(key);
    if (!pair->key)
        return NULL;

    // copy the value onto the kv pair
    pair->value = *val;

    return pair;
};

void kv_free(kv_t* pair)
{
    if (!pair)
        return;

    free(pair->key);
    free(pair);

    return;
};

value_t kv_get(kv_t* pair)
{
    return pair->value;
};

void kv_set(kv_t* pair, const value_t* val)
{
    pair->value = *val;

    return;
};
