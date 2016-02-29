//
// Created by cl-k on 9/28/15.
//
#ifndef __CLION_UTIL_H__
#define __CLION_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "debug.h"

typedef struct _as_conf{
    //
    char* root_path;
    unsigned  short port;
}as_conf;


//this func read the config file
//surely it will be easier to use with cpp
int read_config(const char* filename,char* buff,as_conf* config,int len)
{
    // read config file;
    // get the beginning word;  !strlen ,!'\n'  '\0'
    // get the "=" pos;  !strstr
    // get the lens of a line; pos+=

    int line_len = 0;
    char* cur_pos  = buff;
    char* delim_pos;

    FILE* fp = fopen(filename,"r");
    if(!fp)
    {
        printf("open file error ! \n");
        return -1;
    }
    while(fgets(cur_pos,len,fp))
    {
        delim_pos = strstr(cur_pos,"=");
        line_len = strlen(cur_pos);

        // be sure that '\0' at the end
        if(cur_pos[line_len-1]=='\n')
            cur_pos[cur_len] = '\0';

        if(strncmp(cur_pos,"root",4)==0)
            config->root_path = delim_pos+1;

        if(strncmp(cur_pos,"port",4)==0)
            config->port = atoi(delim_pos+1);

        cur_pos += line_len;
    }

    return 0;
}

#endif
