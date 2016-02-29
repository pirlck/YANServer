//
// Created by cl-k on 9/23/15.
//
// a simple http server 

#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <error.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>					//protocl addr地址协议族相关文件..INTERNET地址族	
#include <sys/ioctl.h>
#include <arpa/inet.h>					//INTERNET定义(协议)
#include <assert.h>
#include "as_debug.h"
#include "as_epoll.h"
#include "as_utils.h"
#include "as_socket.h"
#include "pthread_pool.h"


#define CONFIG "as_serv.conf"
#define BUFLEN 1024
#define OK	   0
#define EPOLLSIZE 256

void print_usage()
{
	fprintf(stdout,
		"YAN server [option]...\n"
		"-? |-h|--help"
		);
}


//will use red black tree implement timer
int as_timer()
{
    int status = 0;
    //will use red black tree.implement timer
    printf("this func need to be implemented\n");
    return status;
}

int main(int argc,char* argv[])
{
	int status;
	char* config = CONFIG; 
	char* optstr = "vc:?";

    struct sockaddr_in cli_addr;
    socklen_t  len_sock;

    struct epoll_event* ep_events;
    struct epoll_event event;

	//must have 2 paraments
	if(argc == 1)
	{
		print_usage();
		return 0;
	}

    //get the command line paraments
	while( (opt = getopt(argc,argv,optstr) )!= -1)
	{
		switch (opt)
		{
		  case 'c':
			 config = optstr;		 break;
		  case 'v':
			 printf("0.1VERSION\n"); break;
		  case '?':
			 print_usage();
			 return 0;
		}
	}

	// print the debug file
	sev_debug("config file: %s \n");

	// read the config file,check if success
	char buff[BUFLEN];
    as_conf* config = (as_conf*)malloc(sizeof(as_conf));

    status = read_config(config,buff,config,BUFLEN);
	if(status != OK)
	{
		log_err("read config file error !");
		exit(-1);
	}

    int listen_fd;
    listen_fd = creat_sockfd(9000);

    status = set_socket_nonblock(listen_fd);
    if(status != OK)
    {
        log_err("set sock fd block err!");
        exit(-1);
    }

    //create epoll ctl fd
    //malloc set of events
	int epfd = as_epoll_create(EPOLLSIZE);
    ep_events = (struct epoll_event*)malloc(EPOLLSIZE*sizeof(struct epoll_event));

    //construct event and register event to the set
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    as_epoll_add(epfd,listen_fd,&event);

    //initial the thread pool
    // the pool use singleton pattern, pool is defined in .h
    init_pool(10);


    //main loop
    while(1)
    {
        int n_eves;
        n_eves = as_epoll_wait(epfd,ep_events,MAXEVESZ,TIMEOUT);

        int i,cli_fd;
        for(i=0;i<n_eves;i++)
        {
            cli_fd = ep_events[i].data.fd;
            if(listen_fd == cli_fd)
            {
                //some new clients ask to accept
                //accept,make_non_block,add_event
                //len_sock
                int fd = accept(listen_fd,(struct sockaddr*)&cli_addr,&len_sock);
                if(fd < 0)
                {
                    //log_err
                    log_err("accept cli fd error ! \n");
                    break;
                }

                //set fd non block
                //check status if normal
                status = set_socket_nonblock(fd);

                //set event paraments
                event.data.fd = fd;
                event.events = EPOLLIN | EPOLLET;

                //add event-->epoll
                as_epoll_add(epfd,fd,&event);

            }
            else
            {
                //accepted client's data ready to do_resquest
                //or need to close
                //how to detect the fd is a close signal
                if( (ep_events[i].events & EPOLLERR)|| (ep_events[i].events & EPOLLHUP)
                    || !(ep_events[i].events & EPOLLIN) )
                {
                    //log_info..need to close this client fd
                    close(fd);
                    continue;
                }
                //log_info  need to do the resquest
                printf("new task client:%d \n",fd);
                status = pool_addtask(do_resquest,&cli_fd);
                printf("task from client:%d  status:%d",fd,status);

            }
        }
        // need to fill timeout func.
        //printf("if timer end,the server will stop !\n");
        if(!as_timer())
        {
            log_info("long time no resquest,this server is about to out of service!");
            break;
        }

    }

    destroy_pool(pool);
    return 0;
}
