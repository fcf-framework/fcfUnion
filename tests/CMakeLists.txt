cmake_minimum_required(VERSION 3.0)

project(fcfunion)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./files/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/files)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../)

add_executable( "tests" 
                findTest.cpp
                cmpTest.cpp
                isTest.cpp
                test.cpp
                orderTest.cpp
                eraseTest.cpp
                streamTest.cpp
                errorTest.cpp
                main.cpp
              )


