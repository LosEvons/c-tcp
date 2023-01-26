#include "threads.h"

pthread_t thread_pool[THREAD_POOL_SIZE]; // Initialize thread pool
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER; 
// Initialize condition to stop threads in loop when asking 
// for connection (limits server CPU usage)

void thread_initialize(){
	for (int i = 0; i < THREAD_POOL_SIZE; i++){
		check_thread(
			pthread_create(
				&thread_pool[i],
				NULL,
				thread_process_connection,
				NULL),
			"Failed to initialize threads.");
	}
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