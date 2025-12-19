#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef TCP_H
#define TCP_H

int     tcp_listen(int port);
int     tcp_accept(int listen_fd);
ssize_t tcp_read(int client_fd, char* buf, size_t max_len);
ssize_t tcp_write(int client_fd, const char* buf, size_t len);
void    tcp_close(int fd);

#endif
