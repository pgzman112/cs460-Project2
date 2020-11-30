# cmake_minimum_required(VERSION <specify CMake version here>)
project(Project2Framework)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project2Framework
        LexicalAnalyzer.cpp
        LexicalAnalyzer.h
        Project2.cpp
        SetLimits.cpp
        SetLimits.h
        SyntacticalAnalyzer.cpp
        SyntacticalAnalyzer.h)
