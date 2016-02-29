
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "as_epoll.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "parse.h"
#include "parse_http.h"
#include "as_socket.h"


#define LEN 10240

using namespace std;

int main(int argc,char* argv[])
{
    char buff[LEN];
    int i;
    int serv_sock;
    struct sockaddr_in serv_addr;
    socklen_t  serv_addr_sz;

    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_port = htons(atoi("9000"));

    serv_addr_sz = sizeof(serv_addr);
    bind(serv_sock,(struct sockaddr*)&serv_addr,serv_addr_sz);

    listen(serv_sock,5);

    tyhp_http_header_t* phttphdr = tyhp_alloc_http_header();

    // void* my_task(void* arg);

    while(1)
    {
        init_pool(3);

        int cli_fd = accept(serv_sock,(struct sockaddr*)&serv_addr,&serv_addr_sz);
        if(cli_fd > 0)
        {
            pool_addtask(do_resquest,&cli_fd);
        }

        sleep(30);
        destroy_pool(pool);
        return 0;

    }

    /*
    while(1)
    {
        int clifd = accept(serv_sock,(struct sockaddr*)&serv_addr,&serv_addr_sz);

        int rev_len = read(clifd,buff,sizeof(buff));

        string http_resquest = buff;
        cout<<"http resquest size:"<<http_resquest.size()<<endl;
        cout<<http_resquest<<endl;
        tyhp_parse_http_request(http_resquest,phttphdr);
        cout<<phttphdr->method<<" "<<phttphdr->url<<" "<<phttphdr->version<<endl;
        tyhp_free_http_header(phttphdr);

    }
    */

    return 0;
}

