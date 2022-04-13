cmake_minimum_required(VERSION 3.20)
project(c_intre_preteur C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(test_unbounded
        test_unbounded.c)