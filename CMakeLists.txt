cmake_minimum_required(VERSION 3.31)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess main.cpp
        minmax.h
        move.cpp
        move.h
        board.cpp
        board.h)
