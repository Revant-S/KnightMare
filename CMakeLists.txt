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
        "search/legal_move_generation/GeneratePseudoLegalMove.cpp"
        "search/legal_move_generation/GeneratePseudoLegalMove.h"
        search/legal_move_generation/LegalMoveFilter.cpp
        search/legal_move_generation/LegalMoveFilter.h
        search/legal_move_generation/MoveFunctions.cpp
        search/legal_move_generation/MoveFunctions.h
        search/alpha_beta_search/Search.cpp
        search/alpha_beta_search/Search.h
        search/evaluation/Evaluation.cpp
        search/evaluation/Evaluation.h
        search/evaluation/EvaluationUtils.cpp
        search/evaluation/EvaluationUtils.h
        TranspositionTable/TransPositionTable.cpp
        TranspositionTable/TransPositionTable.h
        History/MoveHistory.cpp
        History/MoveHistory.h
)
