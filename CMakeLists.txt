cmake_minimum_required(VERSION 3.27)
project(Database)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(Database main.cpp
        Table.h
        Table.cpp
        Row.h
        Row.cpp
        Patterns.cpp
        Patterns.h
        Engine.h
        Engine.cpp
        Database.h
        Database.cpp
)
target_link_libraries(Database fmt)