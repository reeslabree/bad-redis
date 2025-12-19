#include "../include/protocol.h"

/**
 * "\"string contents\"" -> parsed to char*
 * "true" / "false" -> parsed to bool
 * 1.0 -> parsed to float
 * 1 -> parsed to int
 */
static value_t* parse_value(const char* value_primitive)
{
    value_t* val = malloc(sizeof(value_t));
    int      len = strlen(value_primitive);

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
    else if (strchr(value_primitive, ".") != NULL)
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
    char* buf_clone = strdup(buf);
    char* save_ptr;

    request_t* request = malloc(sizeof(request_t));
    value_t*   value   = parse_request(buf);

    if (value == NULL)
        goto error;

    char* cmd = strtok_r(buf_clone, " ", save_ptr);

    if (strcmp(cmd, "PUT"))
    {
        char* key = strtok_r(buf_clone, " ", save_ptr);
        char* val = strtok_r(buf_clone, "\n", save_ptr);

        if (!key || !val)
            goto error;

        request->action       = PUT;
        request->detail.p.key = key;
    }
    else if (strcmp(cmd, "GET"))
    {
        char* key = strtok_r(buf_clone, "\n", save_ptr);

        if (!key)
            goto error;
    }
    else if (strcmp(cmd, "DELETE"))
    {
        char* key = strtok_r(buf_clone, "\n", save_ptr);

        if (!key)
            goto error;
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
