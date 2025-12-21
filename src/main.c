#include "../include/kv.h"
#include "../include/protocol.h"
#include "../include/store.h"
#include "../include/tcp.h"

#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PORT 49925
#define CAPACITY 19
#define BUF_SIZE 1024

static volatile sig_atomic_t running = 1;
static int listen_fd = -1;

static void handle_sigint(int sig)
{
    (void)sig;
    running = 0;
    if (listen_fd >= 0) {
        close(listen_fd);
        listen_fd = -1;
    }
}

typedef struct handle_client_args
{
    int client_fd;
    kv_store_t* store;
} handle_client_args_t;

void* handle_client(void* args)
{
    handle_client_args_t* data = (handle_client_args_t*)args;
    int                   client_fd = data->client_fd;
    kv_store_t*           store = data->store;

    char* buf = malloc(BUF_SIZE);

    ssize_t n =tcp_read(client_fd, buf, BUF_SIZE);

    if (n <= 0) {
        free(buf);
        free(data);
        return NULL;
    }

    buf[n] = '\0';
    request_t* request = parse_request(buf);
    if (request == NULL) {
        free(buf);
        free(data);
        return NULL;
    }

    switch (request->action) {
        case PUT:
            kv_store_set_key(store, request->detail.p.key,
                             &request->detail.p.value);
            tcp_write(client_fd, "OK\n", 3);
            break;
        case GET: {
            const value_t* value = kv_store_get_key(store, request->detail.g.key);
            if (value == NULL) {
                tcp_write(client_fd, "NOT_FOUND\n", 11);
            } else {
                char buf[BUF_SIZE];
                format_response(*value, buf, BUF_SIZE);
                tcp_write(client_fd, buf, strlen(buf));
            }
            break;
        }
        case DELETE:
            kv_store_delete_key(store, request->detail.d.key);
            tcp_write(client_fd, "OK\n", 3);
            break;
        default:
            tcp_write(client_fd, "ERROR\n", 6);
    }

    free(buf);
    free(data);
    return NULL;
}

int main(void)
{
    signal(SIGINT, handle_sigint);

    kv_store_t* store = kv_store_create(CAPACITY);
    listen_fd = tcp_listen(PORT);

    if (listen_fd < 0) {
        perror("Failed to listen on port");
        kv_store_free(store);
        return 1;
    }

    printf("Listening on port %d\n", PORT);

    while (running)
    {
        printf("Waiting for connection...\n");

        int client_fd = tcp_accept(listen_fd);

        if (client_fd < 0) {
            if (!running) {
                break;
            }
            perror("accept failed");
            continue;
        }

        printf("Accepted connection\n");

        pthread_t thread_id;
        handle_client_args_t* args = malloc(sizeof(handle_client_args_t));
        args->client_fd = client_fd;
        args->store = store;

        pthread_create(&thread_id, NULL, handle_client, (void*)args);
        pthread_detach(thread_id);
    }

    if (listen_fd >= 0) {
        tcp_close(listen_fd);
    }
    kv_store_free(store);

    return 0;
}
