cmake_minimum_required(VERSION 3.16)
project(LRU)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/lru
)

add_executable(LRU
         lru/lru.hpp
        test/8.cpp
)




