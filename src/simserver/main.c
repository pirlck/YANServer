/*
* this is simple model for web server
* nonblocking pthread pool epoll
*
*
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#incldue <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <error.h>
#include <fcntl.h>


/*
*  define alls
*  header files
*/
#define BUFFLEN 1024


int main()
{

    /*
    *  1:about sockets
    *  2:accept link fd
    *  3:process loop
    *  4:register epoll
    *  5:wait event to strike
    */
    int listenfd;
    struct sockaddr_in server_addr;
    char* msg[BUFFLEN];


    //PF_INET: AF_used to internet addr
    //SOCK_STREAM:stream protocol
    listenfd = socket(PF_INET,SOCK_STREAM,0);

    if(-1 == listenfd)
    {
        pritnf("error ! \n");
        exit(-1);
    }


    //
    //difference ?
    //memset(&server_addr,0,sizeof(server_addr) );
    bzero(&server_addr,sizeof(server_addr) );
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]) );
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //about to listen port;
    if(-1 == connect(listenfd,(struct sockaddr_in*)&server_addr,sizeof(server_addr) ) )
    {
        printf("error connecting ! \n");
    }

    while(1)
    {

    }



    return 0;
}







