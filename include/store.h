#include "kv.h"
#include <pthread.h>

#ifndef STORE_H
#define STORE_H

typedef struct kv_node
{
    kv_t            kv;
    struct kv_node* next;
} kv_node_t;

typedef struct kv_bucket
{
    kv_node_t*      head;
    pthread_mutex_t lock;
} kv_bucket_t;

typedef struct kv_store
{
    kv_bucket_t* buckets;
    size_t       capacity;
} kv_store_t;

kv_store_t*    kv_store_create(size_t capacity);
void           kv_store_free(kv_store_t* store);
const value_t* kv_store_get_key(kv_store_t* store, const char* key);
void           kv_store_set_key(kv_store_t* store, const char* key, const value_t* val);
void           kv_store_delete_key(kv_store_t* store, const char* key);

#endif
