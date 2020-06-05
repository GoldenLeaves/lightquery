//
// Created by tramboo on 2020/6/5.
//

#include <lightquery/graph_handle.hh>

namespace lightquery
{

lightgraph::Schema GetSchema() {
    lightgraph::Schema schema;
    schema.AddEdgeLabel("knows");
    schema.AddEdgeLabel("likes");
    return schema;
}

void AddRecords(lightgraph::LDB* db) {}

void init_graph_handle() {
    std::string test_db_path = "../data/graph_store";
    graph_handle = new lightgraph::LDB(GetSchema());
    lightgraph::LOptions options;
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present
    options.create_if_missing = true;

    // Open DB
    auto s = graph_handle->Open(options, test_db_path);
    assert(s.ok());
    std::cout << "Open LDB successfully." << std::endl;

    AddRecords(graph_handle);
}

}