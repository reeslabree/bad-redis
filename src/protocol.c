#include "../include/protocol.h"
#include <string.h>

/**
 * "\"string contents\"" -> parsed to char*
 * "true" / "false" -> parsed to bool
 * 1.0 -> parsed to float
 * 1 -> parsed to int
 */
static value_t* parse_value(const char* value_primitive)
{
    value_t* val = malloc(sizeof(value_t));
    size_t   len = strlen(value_primitive);

    if (len > 2 && value_primitive[0] == '"' && value_primitive[len - 1] == '"')
    {
        val->type = STRING;

        if (len - 2 >= sizeof(val->value.str))
            goto error;

        strncpy(val->value.str, value_primitive + 1, len - 2);
        val->value.str[len - 2] = '\0';
    }
    else if (!strcmp(value_primitive, "true") ||
             !strcmp(value_primitive, "false"))
    {
        val->type = BOOL;

        if (!strcmp(value_primitive, "true"))
        {
            val->value.b = true;
        }
        else
        {
            val->value.b = false;
        }
    }
    else if (strchr(value_primitive, '.') != NULL)
    {
        val->type = FLOAT;

        char* endptr;
        float f = strtof(value_primitive, &endptr);

        if (*endptr != '\0')
            goto error;

        val->value.f = f;
    }
    else
    {
        val->type = INT;

        char* endptr;
        int   i = strtol(value_primitive, &endptr, 10);

        if (*endptr != '\0')
            goto error;

        val->value.i = i;
    }

    return val;
error:
    free(val);
    return NULL;
}

request_t* parse_request(const char* buf)
{
    char*    buf_clone = strdup(buf);
    char*    save_ptr;
    value_t* value = NULL;

    request_t* request = malloc(sizeof(request_t));
    char*      key     = malloc(sizeof(char) * 40);
    key[0]             = '\0';

    char* cmd = strtok_r(buf_clone, " \n", &save_ptr);

    if (cmd == NULL)
        goto error;

    if (strcmp(cmd, "PUT") == 0)
    {
        char* key_tok = strtok_r(save_ptr, " ", &save_ptr);
        if (key_tok == NULL)
            goto error;

        strcpy(key, key_tok);

        char* val = strtok_r(save_ptr, "\n", &save_ptr);

        value = parse_value(val);

        if (value == NULL)
            goto error;

        if (strlen(key) == 0 || !val)
            goto error;

        request->action         = PUT;
        request->detail.p.key   = key;
        request->detail.p.value = *value;

        free(value);
    }
    else if (strcmp(cmd, "GET") == 0)
    {
        strcpy(key, strtok_r(save_ptr, "\n", &save_ptr));

        if (strlen(key) == 0)
            goto error;

        request->action       = GET;
        request->detail.g.key = key;
    }
    else if (strcmp(cmd, "DELETE") == 0)
    {
        strcpy(key, strtok_r(save_ptr, "\n", &save_ptr));

        if (strlen(key) == 0)
            goto error;

        request->action       = DELETE;
        request->detail.g.key = key;
    }
    else
    {
        goto error;
    }

    free(buf_clone);
    return request;

error:
    free(buf_clone);
    free(request);
    free(value);
    return NULL;
}

size_t format_response(const value_t val, char* buf, size_t buf_size)
{
    if (buf_size == 0)
        return 0;

    int written = 0;

    switch (val.type)
    {
    case STRING:
        written = snprintf(buf, buf_size, "\"%s\"", val.value.str);
        break;

    case INT:
        written = snprintf(buf, buf_size, "%d", val.value.i);
        break;

    case FLOAT:
        written = snprintf(buf, buf_size, "%f", val.value.f);
        break;

    case BOOL:
        written = snprintf(buf, buf_size, "%s", val.value.b ? "true" : "false");
        break;

    default:
        return 0;
    }

    if (written < 0)
        return 0;

    if ((size_t)written >= buf_size)
        return buf_size - 1;

    return (size_t)written;
}

void free_request(request_t* request)
{
    switch (request->action)
    {
    case PUT:
        free(request->detail.p.key);
        break;
    case GET:
        free(request->detail.g.key);
        break;
    case DELETE:
        free(request->detail.d.key);
        break;
    default:
        break;
    }

    free(request);
}
