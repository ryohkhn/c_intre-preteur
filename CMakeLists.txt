cmake_minimum_required(VERSION 3.2O)
project(c_intre-preteur C)

set(CMAKE_C_STANDARD 11)

add_executable(test_unbounded
        test_unbounded.c
        )

add_executable(calc_unbounded
        calc_unbounded_int.c)
