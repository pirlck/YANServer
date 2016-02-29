//
// Created by cl-k on 10/5/15
//
#ifndef __PARSE_HTTP__
#define __PARSE_HTTP__

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include "pthread_pool.h"


#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: yashttpd/0.1.0\r\n"

// store the parse data from the
// resquest maybe should replace by string;
typedef struct _http_header{
    char method[64];
    char url[512];
    char version[48];
}http_header;

typedef struct _file_type_t{
    const char* type;
    const char* value;
}file_type;


//
int parse_resquest(char* buff,http_header* pheader);

void print_header(http_header* pheader);

//make real request url
int make_real_url(http_header* pheader,char* path,int cli_fd);

//the server static file
int serv_static(int cli_fd,char* filename);

//create a post header
void make_header(int cli_fd,char* filename);

//file not found
void not_found(int cli_fd);

//the request method is not implemented
void unimplemented(int cli_fd);

//get the request file type
const char* get_filetype(const char* type);


#endif

