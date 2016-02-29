//
// Created by cl-k on 9/30/15.
//
#ifndef __YAS_EPOLL__
#define __YAS_EPOLL__

#include <sys/epoll.h>

//Events*  ep_events;
#define OK 0
#define TIMEOUT  -1
#define MAXEVESZ 256


typedef struct epoll_event Events;

int  as_epoll_create(int flag);

void as_epoll_add(int epfd,int fs,Events* event);

void as_epoll_mod(int epfd,int fs,Events* event);

void as_epoll_wait(int epfd,int fs,Events* event);

void as_epoll_del(int epfd,int fs,Events* event);

#endif