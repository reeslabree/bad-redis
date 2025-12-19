#include "../include/kv.h"
#include "../include/protocol.h"
#include "../include/store.h"
#include "../include/tcp.h"

#include <signal.h>
#include <stdio.h>

#define PORT 49925

static volatile sig_atomic_t running = 1;

static void handle_sigint(int sig)
{
    (void)sig;
    running = 0;
}

int main(void)
{
    signal(SIGINT, handle_sigint);

    // int listen_fd = tcp_listen(PORT);

    while (running)
    {
    }

    return 0;
}
