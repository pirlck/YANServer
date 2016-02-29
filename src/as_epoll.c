//
// Created by cl-k on 9/30/15.
//
#include "epoll.h"
#include "debug.h"

//create a epoll 
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

//add the event fd into event
void as_epoll_add(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,event);
	if(status != OK)
	{
		log_err("add event err !");
		exit(-1);
	}
}

void as_epoll_mod(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_MOD,fd,event);
	if(status != OK)
	{
		log_err("")
	}
}

//del the fd 
void as_epoll_del(int epfd,int fd,Events* event)
{
	int status = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,event);
	if(status != OK)
	{
		log_err("delete event err !");
		exit(-1);
	}
}

//epoll wait event
int as_epoll_wait(int epfd,Events* events,int maxevents ,int timeout )
{
    //MAXEVESZ,TIMEOUT
	int timeout = -1;
	int n = epoll_wait(epfd,events,maxevents,timeout);
	if( n < 0)
	{
		log_err("wait epoll error !\n");
		exit(-1);
	}
	return n;
}

