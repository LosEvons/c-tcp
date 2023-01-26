#include "myqueue.h"

static node_t* head = NULL;
static node_t* tail = NULL;

void enqueue(int *client_socket){
	node_t *newnode = malloc(sizeof(node_t));
	newnode->client_socket = client_socket;
	newnode->next = NULL;
	if (tail == NULL) {
		head = newnode;
	} else {
		tail->next = newnode;
	}
	tail = newnode;
}

// Return NULL if queue empty
// Otherwise return pointer to client_socket
int* dequeue(){
	if (head == NULL) {
		return NULL;
	} else {
		int *result = head->client_socket;
		node_t *temp = head;
		head = head->next;
		if (head == NULL) {
			tail = NULL;
		}
		free(temp);
		return result;
	}
}