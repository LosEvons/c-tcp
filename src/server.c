#include "handle_connection.h"

int setup_server(short port, int backlog);

int main(int argc, char **argv){
	int server_socket = setup_server(SERVER_PORT, SERVER_BACKLOG);

	fd_set current_sockets, ready_sockets; // a set of file descriptors
	// Init current set
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);


	while (true){
		// temp copy, since select is destructive
		ready_sockets = current_sockets;

		printf("Waiting for connection...\n");

		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0){
			perror("Select error.");
			exit(EXIT_FAILURE);
		}

		for (int i=0; i < FD_SETSIZE; i++){ 	// Go through all sockets
			if (FD_ISSET(i, &ready_sockets)){	// ^
				if (i == server_socket){
					// Found new connection
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				} else{
					// Process connections
					add_connection_to_queue(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

int setup_server(short port, int backlog){
	int server_socket;
	SA_IN server_addr;

	thread_initialize();

	// Create a socket
	check_socket((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		"Failed to create socket.");

	// Bind that socket to a port
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // Internet address
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Responding to anything
	server_addr.sin_port = htons(SERVER_PORT); // Listened port

	check_socket(bind(server_socket, (SA *) &server_addr, sizeof(server_addr)),
		"Bind failed.");

	// Set socket to listen
	check_socket(listen(server_socket, SERVER_BACKLOG),
		"Listen failed.");

	setHttpHeader(httpHeader);

	return server_socket;
}