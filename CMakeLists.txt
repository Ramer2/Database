cmake_minimum_required(VERSION 3.27)
project(Database)

set(CMAKE_CXX_STANDARD 20)

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