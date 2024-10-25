cmake_minimum_required(VERSION 3.0)
project(example003)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../../)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../config.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


add_library("example003l" SHARED ./library/unionImpl.cpp)
# Declare the FCF_UNION_EXPORT macro for the shared library
target_compile_definitions(example003l PRIVATE FCF_UNION_EXPORT)


add_executable(example003 ./executable/main.cpp)
# Declare the FCF_UNION_IMPORT macro for the executable
target_compile_definitions(example003 PRIVATE FCF_UNION_IMPORT)
target_link_libraries(example003 PRIVATE "example003l")







