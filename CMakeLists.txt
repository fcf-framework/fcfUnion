cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME fcfunion)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../)

add_subdirectory(tests ${CMAKE_BINARY_DIR}/tests)
add_subdirectory(examples/example001 ${CMAKE_BINARY_DIR}/example001)
add_subdirectory(examples/example002 ${CMAKE_BINARY_DIR}/example002)
add_subdirectory(examples/example003 ${CMAKE_BINARY_DIR}/example003)
