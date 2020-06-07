//
// Created by tramboo on 2020/6/5.
//
#pragma once

#include <lightquery/typedef.hh>
#include <iostream>

namespace lightquery
{

lightgraph::Schema GetSchema();

void AddRecords(lightgraph::LDB* db);

void init_graph_handle();

}
