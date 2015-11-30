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

using namespace std;

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
	//server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[2]));

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

	//int server_sock_len;
	socklen_t server_sock_len;
	server_sock_len = sizeof(server_addr);

	//main loop
	while(true)
	{
		//for(i=0;i<5;i++)
		//{
			client_sock = accept(server_sock,(struct sockaddr*)&server_addr,(socklen_t*)&server_sock_len );

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

			status = write(client_sock,msg,sizeof(msg) );

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