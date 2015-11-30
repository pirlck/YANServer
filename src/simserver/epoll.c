#include "epoll.h"
#include "debug.h"

Events*  event;
#define OK 0
#define timeout 10

/*
*
*/
int as_epoll_create(int flag)
{
	int epfd = epoll_create1(flag);
	if(epfd <= 0)
	{
		log_err("create epoll err !");
		exit(-1);
	}
	event = (Events*)malloc(MAXEVESZ*sizeof(Events));
	return epfd;
}

/*
* 
*/
void as_epoll_add(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,event);
	if(status != OK)
	{
		log_err("add event err !");
		exit(-1);
	}
}

void as_epoll_mod(int epfd,int fs,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_MOD,fd,event);
	if(status != OK)
	{
		log_err("")
	}
}

void as_epoll_del(int epfd,int fs,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event);
	if(status != OK)
	{
		log_err("delete event err !");
		exit(-1);
	}
}


int as_epoll_wait(int epfd,int fs,Events* event)
{
	int n = epoll_wait(epfd,event,MAXEVESZ,timeout);
	if( n < 0)
	{
		log_err("wait epoll error !\n");
		exit(-1);
	}
	return n;
}

