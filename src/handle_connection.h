#ifndef HANDLE_CONNECTION_H_
#define HANDLE_CONNECTION_H_

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handle_http.h"
#include "myqueue.h"
#include "threads.h"

void * handle_connection(void *client_socket);
void add_connection_to_queue(int client_socket);
int accept_new_connection(int server_socket);

#endif