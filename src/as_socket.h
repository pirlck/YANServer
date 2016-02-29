//
// Created by cl-k on 9/26/15.
//

#ifndef __AS_SOCKET__
#define __AS_SOCKET__
// socket wrap func
//

#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "debug.h"


int creat_sockfd(unsigned short port)
{
	// start a socket
	int servfd;
	struct sockaddr_in servaddr;
	socklen_t socklen;
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	servfd = socket(PF_INET,SOCK_STREAM,0);
	if(servfd == -1)
	{
		log_err("creaete sock error!");
		exit(-1);
	}

    socklen = sizeof(servaddr);
    int status;

    if((status = bind(servfd,(struct sockaddr*)&servaddr,socklen)) < 0 )
        return -1;

    //set fd non blocking
    if( (status = listen(servfd,5) ) == -1)
        return -1;
    log_info("create servfd! success !");
    return servfd;
}

//make a socket non blocking
int set_socket_nonblock(int sock_fd)
{
    int ctl,status;
    //cmd:get such kind control
    ctl = fcntl(sock_fd,F_GETFL);
    if(ctl == -1)
    {
        log_err("GET file CTL failed !");
        return -1;
    }
    ctl |= O_NONBLOCK;
    status = fcntl(sock_fd,ctl);
    if(status == -1)
    {
        log_err("SET file CTL failed !");
        return -1;
    }

    return 0;
}

#endif





