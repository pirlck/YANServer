#ifndef _YAN_EPOLL_
#define _YAN_EPOLL_

#include <sys/epoll.h>
#define  MAXEVESZ 1024

typedef struct epoll_event Events;

int  as_epoll_create(int flag);
void as_epoll_add(int epfd,int fs,Events* event);
void as_epoll_mod(int epfd,int fs,Events* event);
void as_epoll_wait(int epfd,int fs,Events* event);
void as_epoll_del(int epfd,int fs,Events* event);

#endif