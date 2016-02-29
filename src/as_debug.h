//
// Created by cl-k on 9/25/15.
//
// s simple print func wrap for debug use
#ifndef __AS_DEBUG_FILE__
#define __AS_DEBUG_FILE__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
#define debug(format,...) fprintf(stderr,"DEBUG %s:%d: "format" \n",__FILE__,__LINE__,##__VA_ARGS__);
#else
#define debug(format,...)
#endif

#define clean_errno() (errno == 0 ? "None":strerror(errno))

#define log_err(format,...) fprintf(stderr,"[Error] (%s:%d: errno:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#define log_warn(format,...) fprintf(stderr,"[Warning] (%s:%d: warn:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#define log_info(format,...) fprintf(stderr,"[Info] (%s:%d: info:%s) "format" \n",__FILE__,__LINE__,clean_errno(),##__VA_ARGS__)

#endif