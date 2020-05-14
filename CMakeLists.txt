cmake_minimum_required(VERSION 3.10)

project(lightquery)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_compile_options(-std=c++11)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/)

add_library(lightgraph STATIC IMPORTED)
set_property(TARGET lightgraph PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/liblightgraph.a)

set(TARGET_LINK_FLAGS -lrocksdb -ljsoncpp -ldl -pthread)

add_executable(test_demo tests/demo.cc)

target_link_libraries(test_demo lightgraph ${TARGET_LINK_FLAGS})
