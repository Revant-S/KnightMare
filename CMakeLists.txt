cmake_minimum_required(VERSION 3.31)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess main.cpp
        board/Board.cpp
        board/Board.h
        PreMatchComputations/PreMatchAttackComputation.cpp
        PreMatchComputations/PreMatchAttackComputation.h
        types_constants/types.h
        utils/utils.cpp
        utils/utils.h
        types_constants/constants.h
        tests/tests.cpp
        tests/tests.h
        "search/legal_move_generation/GenerateLeglMove.cpp"
        "search/legal_move_generation/GenerateLeglMove.h"
)
