cmake_minimum_required(VERSION 3.10)

project(lightquery)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_compile_options(-std=c++11)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/)

set(SOURCE_FILES
        include/lightquery/dataflow.hh
        include/lightquery/typedef.hh
        include/lightquery/actor_core.hh
        include/lightquery/actor_reference.hh
        include/lightquery/actor_implementation.hh
        include/lightquery/tag_message.hh
        include/lightquery/op_code_tree.hh
        include/lightquery/operator_base.hh
        include/lightquery/operators.hh
        include/lightquery/util/art.hh
        include/lightquery/util/radix_tree_index.hh
        include/lightquery/util/common_utils.hh
        include/lightquery/graph_handle.hh
        src/dataflow.cc
        src/actor_core.cc
        src/actor_implementation.cc
        src/operator_base.cc
        src/operators.cc
        src/util/art.cc
        src/graph_handle.cc)

add_library(lightgraph STATIC IMPORTED)
set_property(TARGET lightgraph PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/liblightgraph.a)

set(TARGET_LINK_FLAGS -lrocksdb -ljsoncpp -ldl -pthread)

add_executable(test_demo tests/demo.cc)
add_executable(test_radix_tree tests/test_radix_tree.cc ${SOURCE_FILES})

target_link_libraries(test_demo lightgraph ${TARGET_LINK_FLAGS})
target_link_libraries(test_radix_tree lightgraph ${TARGET_LINK_FLAGS})