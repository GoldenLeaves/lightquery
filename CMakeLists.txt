cmake_minimum_required(VERSION 3.10)

project(lightquery)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_compile_options(-std=c++11)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/)

set(SOURCE_FILES
        include/lightquery/util/art.hh
        include/lightquery/util/radix_tree_index.hh
        src/util/art.cc)

add_library(lightgraph STATIC IMPORTED ${SOURCE_FILES})
set_property(TARGET lightgraph PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/liblightgraph.a)

set(TARGET_LINK_FLAGS -lrocksdb -ljsoncpp -ldl -pthread)

add_executable(test_demo tests/demo.cc)
add_executable(test_radix_tree tests/test_radix_tree.cc ${SOURCE_FILES})

target_link_libraries(test_demo lightgraph ${TARGET_LINK_FLAGS})
target_link_libraries(test_radix_tree lightgraph ${TARGET_LINK_FLAGS})