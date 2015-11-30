//
// a simple thread pool demo
// need to package a class
// maybe this code is shit 
// The API may looks ugly
// 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>


//void *pthread_func(void* arg)
//{
//	printf("this is a thread 1 \n");
//	printf("arg:%s \n",(char*)arg );
//	pthread_exit(0);
//}

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

// each thread run loop 
void* pthread_routine(void*  arg);

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
	pool->status = 1;						// pool is active 
	pool->max_thread_num = max_num;			// the num of the pthread
	pool->head_task = NULL;					// set head of the task 
					
	// malloc pthread array
	pool->tid_array = (pthread_t*)malloc(max_num* sizeof(pthread_t));

	for(i=0;i<max_num;i++)
	{
		pthread_create( &( pool->tid_array[i]),NULL,pthread_routine,(void*)i );
		printf("create pthread:%d success ! \n",i);
	}

	printf("create pthread routine success ! \n");
}

int pool_addtask(void* (*process)(void*),void* arg)
{
	//create a task ,malloc 
	//add mutex
	//change the cur_env
	
	//check if the queue num is full
	if(pool->cur_queue_num == pool->max_thread_num )
	{
		printf("the pthread pool is full !\n");
		return -1;
	}
	
	//create task node
	PthreadTasks* task = malloc(sizeof(PthreadTasks));
	task->process = process;
	task->arg = (void*)arg;
	task->next = NULL;			//note !!!

	//
	pthread_mutex_lock( &(pool->lock) );
	PthreadTasks* task_pointer = malloc(sizeof(PthreadTasks));
	task_pointer = pool->head_task;
	
	//2 case:
	if(task_pointer != NULL){
		while(task_pointer->next != NULL)
		{
			task_pointer = task_pointer->next;
		}
		task_pointer->next = task;
	}
	else{							//the task queue if empty
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
			printf("pthread will be exit 0x%x ! \n",pthread_self());
			pthread_exit(NULL);
		}

		//lock and unlock
		//check if the queue empty,and get a task node then process
		pthread_mutex_lock(&(pool->lock));
		
		//if the pool is active and the queue is empty,then the routine will wait
		while( (pool->cur_queue_num == 0) && (pool->status) )
		{
			printf("pthread:0x%x is waiting ! \n",pthread_self());
			pthread_cond_wait(&(pool->task_sig),&(pool->lock));
		}

		printf("pthread:0x%x is starting to work ! \n",pthread_self());

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
		printf("the pool is still active ! \n");
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

void* my_task(void* arg)
{
	printf("thread is 0x%x,working on the task %d  \n",pthread_self(),*(int*)arg);
	sleep(1);
	return NULL;
}

int main(int argc,char* argv[])
{
	int i;
	init_pool(3);

	for(i=0;i<10;i++)
	{
		pool_addtask(my_task,&i);
	}

	//int pool_addtask(void* (*process)(void*),void* arg);
	
	sleep(15);
	destroy_pool(pool);
	return 0;
}