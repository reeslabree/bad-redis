/**
 * TODO: ADD THREAD SAFETY TESTS
 */

#include "../include/store.h"
#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int test_init()
{
    kv_store_t* store = kv_store_create(10);
    ASSERT(store != NULL, "kv_store_create returned NULL");
    kv_store_free(store);

    puts("test_init passed");

    return 0;
}

int test_insert_and_get()
{
    kv_store_t* store = kv_store_create(10);
    ASSERT(store != NULL, "kv_store_create returned NULL");
    value_t val = {.value.i = 10};
    kv_store_set_key(store, "test", &val);

    const value_t* retrieved = kv_store_get_key(store, "test");
    ASSERT(retrieved->value.i == 10, "get int value failed");

    kv_store_free(store);

    puts("test_insert_and_get passed");

    return 0;
}

int test_update()
{
    kv_store_t* store = kv_store_create(10);
    ASSERT(store != NULL, "kv_store_create returned NULL");
    value_t val = {.value.i = 10};
    kv_store_set_key(store, "test", &val);

    const value_t* retrieved = kv_store_get_key(store, "test");
    ASSERT(retrieved->value.i == 10, "get int value failed");

    value_t val2 = {.value.i = 20};
    kv_store_set_key(store, "test", &val2);

    retrieved = kv_store_get_key(store, "test");
    ASSERT(retrieved->value.i == 20, "get int value failed");

    kv_store_free(store);

    puts("test_update passed");

    return 0;
}

int test_delete()
{
    kv_store_t* store = kv_store_create(10);
    ASSERT(store != NULL, "kv_store_create returned NULL");
    value_t val = {.value.i = 10};
    kv_store_set_key(store, "test", &val);

    kv_store_delete_key(store, "test");

    const value_t* retrieved = kv_store_get_key(store, "test");
    ASSERT(retrieved == NULL, "get int value failed");

    kv_store_free(store);

    puts("test_delete passed");

    return 0;
}

int main(void)
{
    int failed = 0;

    failed |= test_init();
    failed |= test_insert_and_get();
    failed |= test_update();
    failed |= test_delete();

    if (failed)
    {
        puts("==> some store tests failed <==");
        return 1;
    }
    else
    {
        puts("==> all store tests passed <==");
        return 0;
    }
}
