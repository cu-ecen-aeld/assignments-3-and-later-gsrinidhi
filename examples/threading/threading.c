#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void sleep_ms(long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    sleep_ms(thread_func_args->wait_to_obtain);
    int ret = pthread_mutex_lock(thread_func_args->mutex);
    if(ret != 0) {
        printf("mutex lock failed\n");
    }
    sleep_ms(thread_func_args->wait_to_release);
    pthread_mutex_unlock(thread_func_args->mutex);
    thread_func_args->thread_complete_success = 1;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data *tdata = malloc(sizeof(struct thread_data));
    if(!tdata) {
        return false;
    }
    tdata->mutex = mutex;
    tdata->thread_complete_success = 0;
    // pthread_mutex_init(tdata->mutex,NULL);
    tdata->wait_to_obtain = wait_to_obtain_ms;
    tdata->wait_to_release = wait_to_release_ms;
    int status = pthread_create(thread,NULL,&threadfunc,(void*)tdata);
    if(status) {
        return false;
    } else {
        return true;
    }    
}

