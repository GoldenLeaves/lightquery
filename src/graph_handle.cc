//
// Created by tramboo on 2020/6/5.
//

#include <lightquery/graph_handle.hh>

namespace lightquery
{

lightgraph::LDB* graph_handle = nullptr;

lightgraph::Schema GetSchema() {
    lightgraph::Schema schema;
    schema.AddEdgeLabel("knows");
    schema.AddEdgeLabel("likes");
    return schema;
}

void AddRecords(lightgraph::LDB* db) {
    lightgraph::LStatus s;
    // Put deltas
    s = db->DeltaPut(
            {1001, "knows", 2001, 2, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1001, "knows", 2001, 10, lightgraph::OpFlag::Delete},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1001, "knows", 2001, 18, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1001, "knows", 2001, 22, lightgraph::OpFlag::Delete},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1001, "knows", 2002, 6, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1001, "knows", 2003, 4, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1001, "knows", 2003, 9, lightgraph::OpFlag::Delete},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1005, "likes", 3001, 6, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1005, "likes", 3001, 11, lightgraph::OpFlag::Delete},
            lightgraph::Properties());
    assert(s.ok());
    s = db->DeltaPut(
            {1005, "likes", 3001, 24, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1008, "knows", 2004, 9, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1009, "likes", 3007, 5, lightgraph::OpFlag::Insert},
            lightgraph::Properties());
    assert(s.ok());

    s = db->DeltaPut(
            {1009, "likes", 3007, 7, lightgraph::OpFlag::Delete},
            lightgraph::Properties());
    assert(s.ok());
}

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

    graph_handle->UsingIndex();
    // AddRecords(graph_handle);
}

}