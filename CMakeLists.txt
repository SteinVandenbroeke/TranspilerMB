cmake_minimum_required(VERSION 3.16)
project(transpiler)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(transpiler main.cpp AstNode.h AstNode.cpp Token.h Token.cpp SymbolTable.cpp SymbolTable.h)
file(COPY testEnv DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
