#!/bin/bash

#build read_config file
gcc read_config.c -o read_config -lpthread

#build thread pool test file 
gcc tpool_test.c -o tpool_test -lpthread
