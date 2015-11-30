/*
* this is simple model for web server
* nonblocking pthread pool epoll
*
* this is client demo;
* ready to write server demo;
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <error.h>
#include <fcntl.h>
#include <string.h>

/*
*  define alls
*  header files
*/
#define BUFFLEN 1024


int main(int argc,char* argv[])
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
    char msg[BUFFLEN];
    int send_len,rev_len,rev_cnt;


    //PF_INET: AF_used to internet addr
    //SOCK_STREAM:stream protocol
    listenfd = socket(PF_INET,SOCK_STREAM,0);

    if(-1 == listenfd)
    {
        printf("error ! \n");
        exit(-1);
    }


    //difference ?
    //memset(&server_addr,0,sizeof(server_addr) );
    bzero(&server_addr,sizeof(server_addr) );
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]) );
    

    //about to listen port;
    if(-1 == connect(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr) ) )
    {
        printf("error connecting ! \n");
    }

    while(1)
    {
        printf("Input message ! (Q to quit) \n");
        fgets(msg,BUFFLEN,stdin);

        // strcmp :if the input is "Q or q \n"!

        send_len = write(listenfd,msg,strlen(msg));

        if(send_len == -1)
        {
            printf("send msg error ! \n");
            exit(-1);
        }
        else
        {
            printf("send str:%d \n",send_len);
        }
		
		memset(msg,0,strlen(msg));
		
        rev_len = 0;
        // while(rev_len < send_len)
        // {
            // rev_cnt = read(listenfd,&msg[rev_len], BUFFLEN-1-rev_len);
            // if(rev_cnt == -1)
            // {
                // printf("read echo data error ! \n");
                // exit(-1);
            // }
            // rev_len += rev_cnt;
        // }
	   //sure the read data len ,or at the end of array,will display uncertain char; 
	   //最好确定要读取的有效字符长度，不然会读取一些不确定的字符，显示出来奇怪的形状
	   //所以要明确 sizeof 和 strlen()两者之间的区别...
	   //sizeof是指内存中占用的空间大小，而strlen()指的是空间中有效的字符
	   
	   //rev_len = read(listenfd,msg,sizeof(msg) );
	   rev_len = read(listenfd,msg,strlen(msg) );
		
       msg[rev_len] = '\0';
       printf("read echo data_len: %d  :%s \n",rev_len,msg);
    }

    close(listenfd);
    return 0;
}

