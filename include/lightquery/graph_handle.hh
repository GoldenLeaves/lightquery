//
// Created by tramboo on 2020/6/5.
//
#pragma once

#include <lightgraph/ldb.hh>
#include <iostream>

namespace lightquery
{

lightgraph::LDB* graph_handle;

lightgraph::Schema GetSchema();

void AddRecords(lightgraph::LDB* db);

void init_graph_handle();

}
