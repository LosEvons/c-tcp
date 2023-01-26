#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
#include "common.h"
#include "myqueue.h"
#include "handle_connection.h"

extern pthread_t thread_pool[THREAD_POOL_SIZE];
extern pthread_mutex_t mutex;
extern pthread_cond_t condition_var;

void thread_initialize();
void * thread_process_connection(void* arg);

#endif