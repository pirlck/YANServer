cmake_minimum_required(VERSION 3.3)
project(hello_clion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES as_erver.c parse_http.h parse_http.c as_epoll.h as_epoll.c as_debug.h pthread_pool.h as_utils.h)


add_executable(hello_clion ${SOURCE_FILES})