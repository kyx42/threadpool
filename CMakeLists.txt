cmake_minimum_required(VERSION 3.5)
project(Threadpool)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        ThreadPool.cpp
        ThreadPool.hpp main.cpp)

add_executable(Threadpool ${SOURCE_FILES})

target_link_libraries(Threadpool pthread)