cmake_minimum_required(VERSION 3.16)
project(transpiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(transpiler main.cpp AstNode.h AstNode.cpp Token.h Token.cpp)
