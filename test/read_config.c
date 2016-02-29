/*************************************************************************
	> File Name: _strstr.c
	> Author: pirlck
	> Mail: cl.kuang@gmail.com 
	> Created Time: Sat 04 Oct 2015 8:32:36 PM PST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define LEN 1024

typedef struct _as_conf{
    char* root_path;
	//atoi()
    unsigned  short port;
}as_conf;

int read_config(const char* filename,char* buff,as_conf* config,int len)
{
	// read config file;
	// get the beginning word;  !strlen ,!'\n'  '\0'
	// get the "=" pos;  !strstr
	// get the lens of a line; pos+=
	FILE* fp = fopen(filename,"r");
	if(!fp)
	{
		printf("open file error ! \n");
		return -1;
	}
	char* delim_pos;
	char* cur_pos = buff;
	int line_lens = 0;
	
	while(fgets(cur_pos,len,fp))
	{
		delim_pos = strstr(cur_pos,"=");
		line_lens = strlen(cur_pos);
		if(!delim_pos)
		{
			printf("find '=' error ! \n");
		    return -1;
		}
		
		if(cur_pos[line_lens-1] == '\n')
			cur_pos[line_lens-1] = '\0';
		if(strncmp(cur_pos,"root",4) == 0)
			config->root_path = delim_pos +1;
		if(strncmp(cur_pos,"port",4) == 0)
			config->port = atoi(delim_pos + 1);
		
		cur_pos += line_lens;
	}
	fclose(fp);
	return 0;
}

int main()
{
	char buff[LEN];
	as_conf* config_ =(as_conf*) malloc(sizeof(as_conf));
	
	read_config("as_serv.conf",buff,config_,1024);
	printf("read the ocnfig:%s \n%d \n",config_->root_path,config_->port);
	return 0;
}
