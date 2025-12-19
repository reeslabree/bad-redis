#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef KV_H
#define KV_H

typedef enum value_type
{
    STRING,
    INT,
    FLOAT,
    BOOL
} value_type_t;

typedef struct value
{
    value_type_t type;
    union
    {
        char  str[40];
        int   i;
        float f;
        bool  b;
    } value;
} value_t;

typedef struct kv
{
    char*   key;
    value_t value;
} kv_t;

kv_t*   kv_create(const char* key, const value_t* val);
void    kv_free(kv_t* pair);
value_t kv_get(kv_t* pair);
void    kv_set(kv_t* pair, const value_t* val);

#endif
