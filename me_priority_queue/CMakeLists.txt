cmake_minimum_required(VERSION 3.15)
project(me_pq)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CC_STANDARD 17)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    ${CMAKE_MODULE_PATH})

add_executable(pqueue pqueue.cc)