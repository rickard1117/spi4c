project(SI)
cmake_minimum_required(VERSION 2.8)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(3RD_DIR "${ROOT_DIR}/thirdparty")
set(GTEST_DIR_INC "${3RD_DIR}/googletest/googletest/include/")
set(GMOCK_DIR_INC "${3RD_DIR}/googletest/googlemock/include/")
set(GTEST_DIR_LIB "${3RD_DIR}/googletest/")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -O2 -Wall -Wno-deprecated")


include_directories(${GTEST_DIR_INC} ${GMOCK_DIR_INC})
link_directories(${GTEST_DIR_LIB})

add_executable(si si.h si.cpp main.cpp)
add_executable(test si.h si.cpp SiTest.cpp)

target_link_libraries(test libgtestall.a pthread)