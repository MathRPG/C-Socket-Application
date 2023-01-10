cmake_minimum_required(VERSION 3.23)
project(first C)

set(CMAKE_C_STANDARD 99)

add_executable(Scratch test.c)
add_executable(Server server.c socket_utils.c socket_utils.h)
add_executable(Client client.c socket_utils.c socket_utils.h)
