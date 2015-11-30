#ifndef __SOCKET__
#define __SOCKET__
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"


int creat_sockfd(unsigned short port)
{
	/*
	* start a socket
	*/
	int servfd;
	unsigned short port;
	struct sockaddr_in servaddr;
	socklen_t socklen;
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	servfd = socket(PF_INET,SOCK_STREAM,0);
	if(servfd = -1)
	{
		log_err("creaete sock error!");
		exit(-1);
	}
	socklen = sizeof(servaddr);
	int status;
	
	if((status = bind(servfd,(struct sockaddr*)&servaddr,socklen)) < 0 )
		return -1;
	/*
	* set fd nonblocking
	*/
	if( (status = listen(servfd,5) == -1)
		return -1;
	log_info("create servfd!");
	return servfd;
}

#endif