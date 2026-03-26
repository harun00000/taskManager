cmake_minimum_required(VERSION 3.10)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(server
    app/main.cpp
    src/handler.cpp
    src/httpRequest.cpp
    src/server.cpp
)