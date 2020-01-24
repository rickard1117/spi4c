project(SI)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(3RD_DIR "${ROOT_DIR}/thirdparty")
set(GTEST_DIR_INC "${3RD_DIR}/googletest/googletest/include/")
set(GMOCK_DIR_INC "${3RD_DIR}/googletest/googlemock/include/")
set(GTEST_DIR_LIB "${3RD_DIR}/googletest/")
set(CMAKE_CXX_FLAGS "-std=gnu++17 -g -O2 -Wall -Wno-deprecated")


include_directories(${GTEST_DIR_INC} ${GMOCK_DIR_INC})
link_directories(${GTEST_DIR_LIB})

set(SRC_FILES AST.h AST.cpp Interpreter.h Interpreter.cpp Lexer.h Lexer.cpp NodeVisitor.h NodeVisitor.cpp Parser.h Parser.cpp)
set(TEST_FILES SiTest.cpp)

add_executable(test ${SRC_FILES} SiTest.cpp)

target_link_libraries(test libgtest.a pthread)