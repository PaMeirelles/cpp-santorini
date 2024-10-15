cmake_minimum_required(VERSION 3.29)
project(cpp_santorini)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

include_directories(.)

add_executable(cpp_santorini
        board.cpp
    board.h
    eval.cpp
    eval.h
    hash.cpp
    hash.h
    hashTable.cpp
    hashTable.h
    main.cpp
        move.cpp
    move.h
        search.cpp
    search.h
    timeManagement.cpp
    timeManagement.h
        defs.h
        interface.cpp
        interface.h
        profiling.cpp
        profiling.h
)
