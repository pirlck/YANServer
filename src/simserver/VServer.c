/*
* this is a server demo
* 
*/
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <string.h>


#define BUFFLEN 1024

//create socket
//initial socket
//bind()--->sock
//listen()--->sock
// client_fd<----accept()
//return fd ...read or write!!!
int main(int argc, char *argv[])
{
	int server_sock,client_sock,i; 
	char msg[BUFFLEN];
	int send_len,rev_len;
	struct sockaddr_in server_addr;

	server_sock = socket(PF_INET,SOCK_STREAM,0);

	if(-1 == server_sock)
	{
		printf("create socket fd error ! \n");
		exit(-1);
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET ;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);			//port number
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[2]));				//local ip

	//remember is the struct sockaddr* ;not sockaddr_in* ; 
	if (-1 == bind( server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) )
	{
		printf("bind socket error ! \n");
		exit(-1);
	}

	//para int n is the vector<socket fd>
	if(-1 == listen(server_sock,5 ) )
	{
		printf("listen socket fd error ! \n");
		exit(-1);
	}

	socklen_t server_sock_len;
	server_sock_len = sizeof(server_addr);

	//阻塞还是非阻塞
	client_sock = accept(server_sock,(struct sockaddr*)&server_addr,(socklen_t*)&server_sock_len );
	
	//main loop
	while(1)
	{
		//for(i=0;i<5;i++)
		//{
			if(client_sock == -1 )
			{
				printf("connection error ! \n");
				exit(-1);
			}

			int status  = read(client_sock,msg,BUFFLEN);

			if(-1 == status)
			{
				printf("read data error ! \n");
				exit(-1);
			}

			//print the rev data !
			printf("server rev data:%s \n",msg);

			status = write(client_sock,msg,strlen(msg) );

			if(-1 == status)
			{
				printf("write data error ! \n");
				exit(-1);
			}
			memset(msg,0,strlen(msg));
		//}
	}
	close(client_sock);
	close(server_sock);

	return 0;
}
