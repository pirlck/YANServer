//
//Created by cl-k on 9/28/15
//

#include "pthread_pool.h"
#include "as_debug.h"

//max num of threads
void init_pool(int max_num)
{
    //malloc _memory for pointer
    //create _pthread array
    //init _mutex && _cond
    //run each _pthread
    int i;
    pool = (Thread_pool*)malloc(sizeof(Thread_pool));
    pthread_mutex_init( &(pool->lock),NULL);
    pthread_cond_init(&(pool->task_sig),NULL);

    // set all the flag for the pthread pool
    pool->status = true;						// pool is active
    pool->max_thread_num = max_num;			// the num of the pthread
    pool->head_task = NULL;					// set head of the task

    // malloc pthread array
    pool->tid_array = (pthread_t*)malloc(max_num* sizeof(pthread_t));

    for(i=0;i<max_num;i++)
    {
        pthread_create( &( pool->tid_array[i]),NULL,pthread_routine,(void*)i );
        log_info("create pthread:%d success !",i);
    }

    log_info("create pthread routine success !");
}

int pool_addtask(void* (*process)(void*),void* arg)
{
    //create a task ,malloc
    //add mutex
    //change the cur_env

    //check if the queue num is full
    if(pool->cur_queue_num == pool->max_thread_num )
    {
        log_info("the pthread pool is full !");
        return -1;
    }

    //create task node
    PthreadTasks* task = (PthreadTasks*)malloc(sizeof(PthreadTasks));
    task->process = process;
    task->arg = (void*)arg;
    task->next = NULL;		//note !!!

    //
    pthread_mutex_lock( &(pool->lock) );
    PthreadTasks* task_pointer = (PthreadTasks*)malloc(sizeof(PthreadTasks));
    task_pointer = pool->head_task;

    //2 case:
    if(task_pointer != NULL){
        while(task_pointer->next != NULL)
        {
            task_pointer = task_pointer->next;
        }
        task_pointer->next = task;
    }
    else{					//the task queue if empty
        pool->head_task = task;
    }

    //debug
    assert(pool->head_task != NULL);

    pool->cur_queue_num++;
    pthread_mutex_unlock(&(pool->lock));
    //post a signal that task queue if not empty
    pthread_cond_signal(&(pool->task_sig));

    return 0;
}

//threads routine
void* pthread_routine(void* arg)
{
    //while loop
    //the routine func run all the active pthread time
    //can be broadcast by the pool,cond
    //how to echo the none empty task queue
    //how to echo the pool destroyed signal
    //how to use mutex when add a task

    printf("starting running pthread is: %d \n",arg);			// pthread_self()

    while(1)
    {
        //the pool is about to destroyed
        if( !pool->status )
        {
            //pthread_mutex_unlock(&(pool->lock));
            log_info("pthread will be exit 0x%x ! ",pthread_self());
            pthread_exit(NULL);
        }

        //lock and unlock
        //check if the queue empty,and get a task node then process
        pthread_mutex_lock(&(pool->lock));

        //if the pool is active and the queue is empty,then the routine will wait
        while( (pool->cur_queue_num == 0) && (pool->status) )
        {
            log_info("pthread:0x%x is waiting ! \n",pthread_self());
            pthread_cond_wait(&(pool->task_sig),&(pool->lock));
        }

        log_info("pthread:0x%x is starting to work ! \n",pthread_self());

        //debug the current var
        assert(pool->cur_queue_num != 0);
        assert(pool->head_task != NULL );

        //get a task from task queue
        pool->cur_queue_num--;
        PthreadTasks* task = pool->head_task;
        pool->head_task = task->next;
        pthread_mutex_unlock(&(pool->lock));

        //process the task
        (*(task->process))(task->arg);
        free(task);
        task = NULL;
    }

    pthread_exit(NULL);
}


void destroy_pool(Thread_pool* pool)
{
    //free all the malloc memory
    //join all the pthread
    //mutex and cond
    //free task queue
    //set the pool NULL

    //make sure the status is right
    if(pool->status)
    {
        log_err("the pool is still active !");
        exit(-1);
    }

    // broadcast all the active pthread
    pthread_cond_broadcast( &(pool->task_sig) );

    //wait all the pthread ;not to be deamon pthread
    //and free the malloc memory
    int i;
    for(i=0;i<pool->max_thread_num;i++)
    {
        //pthread_join( &(pool->tid_array[i]),NULL);
        pthread_join( pool->tid_array[i],NULL);
    }
    free(pool->tid_array);

    //free task queue,need a pointer
    PthreadTasks* task_pointer =NULL;
    while(pool->head_task != NULL)
    {
        task_pointer = pool->head_task;
        pool->head_task = pool->head_task->next;
        free(task_pointer);
    }

    //destroy all the mutex
    pthread_mutex_destroy( &(pool->lock) );
    pthread_cond_destroy( &(pool->task_sig));
    free(pool);
    pool = NULL;
}

//each thread works call_back func
void* my_task(void* arg)
{
    log_info("thread is 0x%x,working on the task %d  \n",pthread_self(),*(int*)arg);
    sleep(1);
    return NULL;
}
