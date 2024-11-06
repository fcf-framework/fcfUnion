cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME fcfunion)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
  message("MSVC")
elseif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../)

add_subdirectory(tests ${CMAKE_BINARY_DIR}/tests)
add_subdirectory(examples/example001 ${CMAKE_BINARY_DIR}/example001)
add_subdirectory(examples/example002 ${CMAKE_BINARY_DIR}/example002)
add_subdirectory(examples/example003 ${CMAKE_BINARY_DIR}/example003)
