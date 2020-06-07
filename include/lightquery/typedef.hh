//
// Created by tramboo on 2020/6/1.
//
#pragma once

#include <lightquery/util/common_utils.hh>
#include <cstdint>
#include <lightgraph/typedef.hh>
#include <lightgraph/ldb.hh>

namespace lightquery
{

typedef __uint8_t byte_t;

using vertex_id_t = lightgraph::__vertex_id_t;
using time_t = lightgraph::__time_t;

struct light_node {
    vertex_id_t vertex;
    time_t time;

    light_node()
        : vertex(0), time(0) {}
    light_node(vertex_id_t vertex_id, time_t location_time)
        : vertex(vertex_id), time(location_time) {}
    ~light_node() = default;
};

enum op_name {
    GET_VERTEX = 0,
    SCAN_VERTEX = 1,
    OUT = 2,
    COUNT = 3,
    LIMIT = 4,
    WHERE_ENTER = 40,
    WHERE_OUT = 41,
    WHERE_COUNT = 42,
    WHERE_IS_GT = 43,
    WHERE_LEAVE = 44,

};

using operator_id_t = int16_t;
using branch_id_t = uint32_t;
// op_name, downstream op id, variadic params.
using code_t = std::tuple<op_name, operator_id_t, std::string>;

const uint32_t shard_number = 1;

extern lightgraph::LDB* graph_handle;

} // end namespace lightquery
