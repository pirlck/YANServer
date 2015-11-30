
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "epoll.h"

using namespace std;

int main()
{
    int epfd = as_epoll_create(0);
    struct  epoll_event event;

    


    return 0;
}
