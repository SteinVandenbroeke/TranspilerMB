cmake_minimum_required(VERSION 3.16)
project(transpiler)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

set(PARSERGENDIR LR1ParserGenerator)
add_executable(parser_generator
        ${PARSERGENDIR}/main.cpp
        ${PARSERGENDIR}/kdb_facilities.h
        ${PARSERGENDIR}/json.hpp
        ${PARSERGENDIR}/Grammar.cpp
        ${PARSERGENDIR}/Grammar.h
        ${PARSERGENDIR}/Symbol.cpp
        ${PARSERGENDIR}/Symbol.h
        ${PARSERGENDIR}/Item.cpp
        ${PARSERGENDIR}/Item.h
        ${PARSERGENDIR}/lr1_parse_table_gen.cpp
        ${PARSERGENDIR}/lr1_parse_table_gen.h
        ${PARSERGENDIR}/ImmutableHashable.cpp
        ${PARSERGENDIR}/ImmutableHashable.h
        ${PARSERGENDIR}/LR1ParseTableGenerator.cpp
        ${PARSERGENDIR}/LR1ParseTableGenerator.h
        )
target_include_directories(parser_generator PUBLIC LR1ParserGenerator)


add_executable(transpiler main.cpp AstNode.h AstNode.cpp tokenizer/Token.h tokenizer/Token.cpp SymbolTable.cpp SymbolTable.h tokenizer/ENFA.h tokenizer/ENFA.cpp tokenizer/Tokenizer.cpp tokenizer/Tokenizer.h CSTNode.h CSTNode.cpp)

add_executable(webInterface WebInterface/main.cpp AstNode.h AstNode.cpp tokenizer/Token.h tokenizer/Token.cpp SymbolTable.cpp SymbolTable.h tokenizer/ENFA.h tokenizer/ENFA.cpp tokenizer/Tokenizer.cpp tokenizer/Tokenizer.h CSTNode.h CSTNode.cpp WebInterface/crow.h WebInterface/WebServer.cpp WebInterface/WebServer.h)



file(COPY testEnv DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY inputs DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")