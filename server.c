#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include "myqueue.h"

#define SERVER_PORT			18000
#define BUFSIZE				4096
#define PATH_MAX			256
#define SOCKETERROR 		(-1)
#define SERVER_BACKLOG		100
#define THREAD_POOL_SIZE	20

pthread_t thread_pool[THREAD_POOL_SIZE]; // Initialize threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER; // Initialize condition to stop
														//	threads in loop when asking for connection
														//	(limits server CPU usage)
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void * handle_connection(void *client_socket);
int check(int ext, const char *msg);
void * thread_process_connection(void *arg);
int setup_server(short port, int backlog);
int accept_new_connection(int server_socket);
void add_connection_to_queue(int client_socket);

int main(int argc, char **argv){

	int server_socket = setup_server(SERVER_PORT, SERVER_BACKLOG);

	while (true){
		printf("Waiting for connections...\n");

		// Accept connections
		
		int client_socket = accept_new_connection(server_socket);

		printf("Connected!\n");

		// Process connections

		add_connection_to_queue(client_socket);
	}

	return 0;
}

int setup_server(short port, int backlog){
	int server_socket;
	SA_IN server_addr, client_addr;

	for (int i = 0; i < THREAD_POOL_SIZE; i++){
		pthread_create(&thread_pool[i], NULL, thread_process_connection, NULL);
	}

	// Create a socket
	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		"Failed to create socket.");

	// Bind that socket to a port
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // Internet address
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Responding to anything
	server_addr.sin_port = htons(SERVER_PORT); // Listened port

	check(bind(server_socket, (SA *) &server_addr, sizeof(server_addr)),
		"Bind failed.");

	// Set socket to listen
	check(listen(server_socket, SERVER_BACKLOG),
		"Listen failed.");

	return server_socket;
}

int accept_new_connection(int server_socket){
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	check(client_socket = 
		accept(server_socket,
			(SA*)&client_addr,
			(socklen_t*)&addr_size),
		"Accept failed.");
	return client_socket;
}

void add_connection_to_queue(int client_socket){
	int *pclient = malloc(sizeof(int));
	*pclient = client_socket;
	pthread_mutex_lock(&mutex);
	enqueue(pclient);
	pthread_cond_signal(&condition_var);
	pthread_mutex_unlock(&mutex);
}

void * thread_process_connection(void *arg){
	while (true){
		int *pclient;
		pthread_mutex_lock(&mutex);
		// If no work in queue, set thread to wait
		if ((pclient = dequeue()) == NULL){
			pthread_cond_wait(&condition_var, &mutex);
			// Try again
			pclient = dequeue();
		}
		pthread_mutex_unlock(&mutex);
		
		if (pclient != NULL) {
			// Connection found
			handle_connection(pclient);
		}
	}
}

void * handle_connection(void* p_client_socket) {
	int client_socket = *((int*)p_client_socket);
	free(p_client_socket); // not needed after this point
	char buffer[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
	char actualpath[PATH_MAX+1];

	// Read the client's message -- the name of the file to read
	while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer)-msgsize-1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
	}
	check(bytes_read, "Recv error.");
	buffer[msgsize-1] = 0; // null terminate message and remove \n

	printf("REQUEST: %s\n", buffer);
	fflush(stdout);

	// Check validity
	if (realpath(buffer, actualpath) == NULL){
		printf("ERROR(bad path): %s\n", buffer);
		close(client_socket);
		return NULL;
	}

	// read file and send contents to client
	FILE *fp = fopen(actualpath, "r");
	if (fp == NULL){
		printf("ERROR(open): %s}n", buffer);
		close(client_socket);
		return NULL;
	}

	while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0){
		printf("sending %zu bytes\n", bytes_read);
		write(client_socket, buffer, bytes_read);
	}

	close(client_socket);
	fclose(fp);
	printf("Closing connection.\n");

	return NULL;
}

int check(int exp, const char *msg){
	if (exp == SOCKETERROR) {
		perror(msg);
		exit(1);
	}
	return exp;
}