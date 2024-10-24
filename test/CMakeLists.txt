cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME fcfunion)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../)

add_library("${PROJECT_NAME}-shared" SHARED shared.cpp sharedUnit.cpp)

set(TEST_CPP findTest.cpp
             cmpTest.cpp
             isTest.cpp
             test.cpp
             orderTest.cpp
             eraseTest.cpp
          )

add_executable("${PROJECT_NAME}-stest" ${TEST_CPP} main.cpp)

target_link_libraries("${PROJECT_NAME}-stest" PRIVATE  "${PROJECT_NAME}-shared")
target_compile_definitions("${PROJECT_NAME}-stest" PRIVATE  FCF_UNION_SHARED=1)

add_executable("${PROJECT_NAME}-test" ${TEST_CPP} main.cpp)

add_executable("${PROJECT_NAME}-exemple001" example001.cpp)



