#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>

#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define SERVER_PORT	18000
#define BUFSIZE		4096

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);
int check(int ext, const char *msg);

#endif