cmake_minimum_required(VERSION 3.31)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess main.cpp
        board.cpp
        board.h
        movegen.cpp
        movegen.h
        types.h
        utils.cpp
        utils.h
        constants.h
)
