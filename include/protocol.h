#include "kv.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum request_action
{
    PUT,
    GET,
    DELETE,
} request_action_t;

typedef struct put_request
{
    char*   key;
    value_t value;
} put_request_t;

typedef struct get_request
{
    char* key;
} get_request_t;

typedef struct delete_request
{
    char* key;
} delete_request_t;

typedef struct request
{
    request_action_t action;
    union
    {
        put_request_t    p;
        get_request_t    g;
        delete_request_t d;
    } detail;
} request_t;

typedef enum response
{
    OK,
    NOT_FOUND,
    ERROR
} response_t;

request_t* parse_request(const char* buf);
size_t     format_response(const value_t value, char* buf, size_t buf_size);
void       free_request(request_t* request);

#endif
