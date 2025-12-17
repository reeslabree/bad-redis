#include "../include/kv.h"
#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int test_create_str()
{
    const char* KEY = "test key";

    value_t val = {.str = "test string"};
    kv_t*   kv  = kv_create(KEY, &val);

    ASSERT(kv != NULL, "kv_create returned NULL");
    ASSERT(strcmp(kv->key, KEY) == 0, "incorrect key");
    ASSERT(strcmp(kv->value.str, "test string") == 0, "incorrect value");

    kv_free(kv);

    puts("test_create_str passed");

    return 0;
}

int test_create_int()
{
    const char* KEY = "int key";

    value_t val = {.i = 42};
    kv_t*   kv  = kv_create(KEY, &val);

    ASSERT(kv != NULL, "kv_create returned NULL");
    ASSERT(strcmp(kv->key, KEY) == 0, "incorrect key");
    ASSERT(kv->value.i == 42, "incorrect value");

    kv_free(kv);

    puts("test_create_int passed");

    return 0;
}

int test_create_float()
{
    const char* KEY = "float key";

    value_t val = {.f = 3.14f};
    kv_t*   kv  = kv_create(KEY, &val);

    ASSERT(kv != NULL, "kv_create returned NULL");
    ASSERT(strcmp(kv->key, KEY) == 0, "incorrect key");
    ASSERT(kv->value.f == 3.14f, "incorrect value");

    kv_free(kv);

    puts("test_create_float passed");

    return 0;
}

int test_create_bool()
{
    const char* KEY = "bool key";

    value_t val = {.b = true};
    kv_t*   kv  = kv_create(KEY, &val);

    ASSERT(kv != NULL, "kv_create returned NULL");
    ASSERT(strcmp(kv->key, KEY) == 0, "incorrect key");
    ASSERT(kv->value.b == true, "incorrect value");

    kv_free(kv);

    puts("test_create_bool passed");

    return 0;
}

int test_set()
{
    const char* KEY = "set test key";

    value_t val1 = {.i = 10};
    kv_t*   kv   = kv_create(KEY, &val1);

    ASSERT(kv != NULL, "kv_create returned NULL");
    ASSERT(kv->value.i == 10, "initial value incorrect");

    value_t val2 = {.i = 20};
    kv_set(kv, &val2);
    ASSERT(kv->value.i == 20, "set int value failed");

    value_t val3 = {.f = 2.5f};
    kv_set(kv, &val3);
    ASSERT(kv->value.f == 2.5f, "set float value failed");

    value_t val4 = {.b = false};
    kv_set(kv, &val4);
    ASSERT(kv->value.b == false, "set bool value failed");

    value_t val5 = {.str = "new string"};
    kv_set(kv, &val5);
    ASSERT(strcmp(kv->value.str, "new string") == 0, "set string value failed");

    kv_free(kv);

    puts("test_set passed");

    return 0;
}

int test_get()
{
    const char* KEY = "get test key";

    value_t val1 = {.i = 100};
    kv_t*   kv   = kv_create(KEY, &val1);

    ASSERT(kv != NULL, "kv_create returned NULL");

    value_t retrieved = kv_get(kv);
    ASSERT(retrieved.i == 100, "get int value failed");

    value_t val2 = {.f = 9.99f};
    kv_set(kv, &val2);
    retrieved = kv_get(kv);
    ASSERT(retrieved.f == 9.99f, "get float value failed");

    value_t val3 = {.b = true};
    kv_set(kv, &val3);
    retrieved = kv_get(kv);
    ASSERT(retrieved.b == true, "get bool value failed");

    value_t val4 = {.str = "get test"};
    kv_set(kv, &val4);
    retrieved = kv_get(kv);
    ASSERT(strcmp(retrieved.str, "get test") == 0, "get string value failed");

    kv_free(kv);

    puts("test_get passed");

    return 0;
}

int test_free()
{
    const char* KEY = "free test key";

    value_t val = {.i = 1};
    kv_t*   kv  = kv_create(KEY, &val);

    ASSERT(kv != NULL, "kv_create returned NULL");

    kv_free(kv);

    kv_free(NULL);

    puts("test_free passed");

    return 0;
}

int main(void)
{
    int failed = 0;

    failed |= test_create_str();
    failed |= test_create_int();
    failed |= test_create_float();
    failed |= test_create_bool();
    failed |= test_set();
    failed |= test_get();
    failed |= test_free();

    if (failed)
    {
        puts("==> some kv tests failed <==");
        return 1;
    }
    else
    {
        puts("==> all kv tests passed <==");
        return 0;
    }
}
