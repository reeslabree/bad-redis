#include "../include/kv.h"
#include "../include/protocol.h"

#include <stdio.h>

int main(void)
{
    request_action_t a = PUT;

    value_t val  = {.str = "cool value"};
    kv_t*   kv_1 = kv_create("my special key", &val);
    puts(kv_1->key);
    puts(kv_1->value.str);
    return 0;
}
