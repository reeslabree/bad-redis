#include "../include/tcp.h"

/**
 * @param port port to init on
 * @returns `int listen_fd` file descriptor for socket
 */
int tcp_listen(int port)
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));

    listen(listen_fd, SOMAXCONN);

    return listen_fd;
}

/**
 * @param listen_fd socket file descriptor
 * @returns `int client_fd` client connection file descriptor
 */
int tcp_accept(int listen_fd)
{
    int client_fd = accept(listen_fd, NULL, NULL);

    return client_fd;
}

/**
 * @param client_fd client file descriptor
 * @param buf buffer to read into
 * @param max_len max length to read
 * @returns `ssize_t n` bytes read
 */
ssize_t tcp_read(int client_fd, char* buf, size_t max_len)
{
    ssize_t n = read(client_fd, buf, max_len);

    return n;
}

/**
 * @param client_fd client file descriptor
 * @param buf buffer to read to socket
 * @param len length to write
 * @returns `ssize_t n` bytes written
 */
ssize_t tcp_write(int client_fd, const char* buf, size_t len)
{
    ssize_t n = write(client_fd, buf, len);

    return n;
}

/**
 * @param client_fd client file descriptor
 */
void tcp_close(int client_fd)
{
    close(client_fd);
}
