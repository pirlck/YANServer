//
//Created by cl-k on 9/28/15
//

#ifndef __CLION_PTHREAD_POOL_H__
#define __CLION_PTHREAD_POOL_H__

//
// a simple thread pool demo
// need to package a class
// maybe this code is shit
// The APIs may looks ugly
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>


// task list
// link node(1:call_func();2:arg)
// next pointer
// can used any models
typedef struct Tasks{
    //call back func para
    void* arg;

    //call back func,when task added,this func will be called
    void* (*process)(void* arg);

    //error when is Tasks*
    struct Tasks* next;
}PthreadTasks;

typedef struct{
    // mutex and cond to ctl the operations
    // of the task queue,and the var must be extern
    pthread_mutex_t lock;
    pthread_cond_t task_sig;

    //the head of the task queue
    PthreadTasks* head_task;

    // _pthread _arry
    pthread_t* tid_array;

    //the pool is destroy or not
    int status;

    // max num_pthreads in the pool
    int max_thread_num;

    // ready tasks in the queue;
    int cur_queue_num;
}Thread_pool;

static Thread_pool* pool = NULL;

// like a list add task
int pool_addtask(void* (*process)(void*),void* arg);

// each thread run loop yield
void* pthread_routine(void*  arg);

//initlize the thread pool
void init_pool(int max_num);

//add task into pool
int pool_addtask(void* (*process)(void*),void* arg);

void* pthread_routine(void* arg);

void destroy_pool(Thread_pool* pool);

void* my_task(void* arg);

#endif
