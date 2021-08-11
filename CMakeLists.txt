cmake_minimum_required(VERSION 3.20)
project(42_exams_2021 C)

set(CMAKE_C_STANDARD 11)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

add_executable(42_exams_2021
        Rank_04/microshell/microshell.c)
