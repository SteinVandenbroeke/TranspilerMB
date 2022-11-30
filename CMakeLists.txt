cmake_minimum_required(VERSION 3.16)
project(transpiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(transpiler main.cpp AstNode.h AstNode.cpp Token.h Token.cpp)
file(COPY testEnv DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
