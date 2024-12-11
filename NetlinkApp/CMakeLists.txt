cmake_minimum_required(VERSION 3.10)

project(NetlinkApp C)

set(CMAKE_C_STANDARD 11)

add_executable(NetlinkApp main.c netlink.c thread.c)

target_link_libraries(NetlinkApp pthread)

