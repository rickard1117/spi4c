project(SI)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(3RD_DIR "${ROOT_DIR}/thirdparty")
set(GTEST_DIR_INC "${3RD_DIR}/googletest/googletest/include/")
set(GMOCK_DIR_INC "${3RD_DIR}/googletest/googlemock/include/")
set(GTEST_DIR_LIB "${3RD_DIR}/googletest/")
set(CMAKE_CXX_FLAGS "-std=gnu++17 -g -O2 -Wall -Wno-deprecated")


include_directories(${GTEST_DIR_INC} ${GMOCK_DIR_INC})
link_directories(${GTEST_DIR_LIB})


set(SRC_FILES Token.h 
Token.cpp 
Lexer.h 
Lexer.cpp 
AST.h 
AST.cpp 
Parser.h 
Parser.cpp 
Interpreter.h
Interpreter.cpp
Symbol.h
SymbolTable.h
SymbolTable.cpp
SymbolTableBuilder.h
SymbolTableBuilder.cpp
util.h 
util.cpp
Type.h
Type.cpp)

ADD_LIBRARY(si SHARED ${SRC_FILES})

add_executable(LexerTest LexerTest.cpp)
target_link_libraries(LexerTest si libgtest.a pthread)

add_executable(InterpreterTest InterpreterTest.cpp)
target_link_libraries(InterpreterTest si libgtest.a pthread)

add_executable(TypeTest TypeTest.cpp)
target_link_libraries(TypeTest si libgtest.a pthread)
