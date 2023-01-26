#ifndef COMMON_H_
#define COMMON_H_

#include "error.h"

#define SERVER_PORT			18000
#define BUFSIZE				4096
#define PATH_MAX			256
#define SOCKETERROR 		(-1)
#define SERVER_BACKLOG		100
#define THREAD_POOL_SIZE	20

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#endif