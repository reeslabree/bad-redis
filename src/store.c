#include "../include/store.h"

kv_store_t* kv_store_create(size_t capacity)
{
    // create buckets
    kv_bucket_t* buckets = malloc(capacity * sizeof(kv_bucket_t));

    // initialize each bucket with empty head
    for (size_t i = 0; i < capacity; i++)
    {
        kv_bucket_t* bucket = &buckets[i];

        bucket->head = NULL;
        if (pthread_mutex_init(&bucket->lock, NULL) != 0)
        {
            // delete previous initialized buckets if this
            // bucket fails to initialize
            for (size_t j = 0; j < i; j++)
            {
                pthread_mutex_destroy(&buckets[j].lock);
            }
            free(buckets);
            return NULL;
        }
    }

    kv_store_t* store = malloc(sizeof(kv_store_t));
    store->buckets    = buckets;
    store->capacity   = capacity;

    return store;
}

static void kv_bucket_free(kv_bucket_t* bucket)
{
    kv_node_t* curr = bucket->head;
    while (curr != NULL)
    {
        kv_node_t* next = curr->next;
        free(curr->kv.key);
        free(curr);
        curr = next;
    }
    pthread_mutex_destroy(&bucket->lock);
    return;
}

void kv_store_free(kv_store_t* store)
{
    // free all nodes in buckets
    for (size_t i = 0; i < store->capacity; i++)
    {
        kv_bucket_free(&store->buckets[i]);
    }

    // free all buckets
    free(store->buckets);

    // free the store
    free(store);

    return;
}

/**
 * djb2 hash function from
 * Mr. Daniel Bernstein
 */
static size_t hash_to_index(const char* key, size_t capacity)
{
    size_t hash = 5381;
    int    c;

    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + (size_t)c;
    }

    return hash % capacity;
}

const value_t* kv_store_get_key(kv_store_t* store, const char* key)
{
    size_t bucket_index = hash_to_index(key, store->capacity);

    // wait to acquire the lock for this bucket
    pthread_mutex_lock(&store->buckets[bucket_index].lock);

    kv_node_t* curr = store->buckets[bucket_index].head;
    if (curr == NULL)
    {
        pthread_mutex_unlock(&store->buckets[bucket_index].lock);
        return NULL;
    }
    kv_node_t* next = curr->next;

    while (curr != NULL)
    {
        if (strcmp(key, curr->kv.key) == 0)
        {
            pthread_mutex_unlock(&store->buckets[bucket_index].lock);

            return &curr->kv.value;
        }

        curr = next;
        if (curr != NULL)
        {
            next = curr->next;
        }
    }

    pthread_mutex_unlock(&store->buckets[bucket_index].lock);
    return NULL;
};

void kv_store_set_key(kv_store_t* store, const char* key, const value_t* val)
{
    size_t bucket_index = hash_to_index(key, store->capacity);

    pthread_mutex_lock(&store->buckets[bucket_index].lock);

    kv_node_t* curr = store->buckets[bucket_index].head;

    if (curr == NULL)
    {
        kv_node_t* new_node               = malloc(sizeof(kv_node_t));
        new_node->kv.key                  = strdup(key);
        new_node->kv.value                = *val;
        new_node->next                    = NULL;
        store->buckets[bucket_index].head = new_node;
        pthread_mutex_unlock(&store->buckets[bucket_index].lock);
        return;
    }

    while (curr != NULL)
    {
        if (strcmp(key, curr->kv.key) == 0)
        {
            kv_set(&curr->kv, val);
            pthread_mutex_unlock(&store->buckets[bucket_index].lock);
            return;
        }

        if (curr->next == NULL)
        {
            break;
        }
        curr = curr->next;
    }

    kv_node_t* new_node = malloc(sizeof(kv_node_t));
    new_node->kv.key    = strdup(key);
    new_node->kv.value  = *val;
    new_node->next      = NULL;
    curr->next          = new_node;

    pthread_mutex_unlock(&store->buckets[bucket_index].lock);
    return;
};

void kv_store_delete_key(kv_store_t* store, const char* key)
{
    size_t bucket_index = hash_to_index(key, store->capacity);

    pthread_mutex_lock(&store->buckets[bucket_index].lock);

    kv_node_t* prev = NULL;
    kv_node_t* curr = store->buckets[bucket_index].head;

    while (curr != NULL)
    {
        if (strcmp(curr->kv.key, key) == 0)
        {
            if (prev == NULL)
            {
                store->buckets[bucket_index].head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }

            free(curr->kv.key);
            free(curr);
            pthread_mutex_unlock(&store->buckets[bucket_index].lock);
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&store->buckets[bucket_index].lock);
    return;
};
