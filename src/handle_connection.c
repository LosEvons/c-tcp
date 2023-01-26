#include "handle_connection.h"

int accept_new_connection(int server_socket){
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	check_socket(client_socket = 
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
	check_socket(bytes_read, "Recv error.");
	buffer[msgsize-1] = 0; // null terminate message and remove \n

	printf("REQUEST: %s\n", buffer);
	fflush(stdout);



	// check_socket validity
	if (realpath(buffer, actualpath) == NULL){
		printf("ERROR(bad path): %s\n", buffer);
		send(client_socket, httpHeader, sizeof(httpHeader), 0);
		close(client_socket);
		return NULL;
	} else {
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

		fclose(fp);
	}

	close(client_socket);
	printf("Closing connection.\n");

	return NULL;
}