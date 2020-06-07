//
// Created by tramboo on 2020/6/7.
//

#include <lightquery/dataflow.hh>
#include <lightquery/graph_handle.hh>

using namespace lightquery;

int main() {
    init_graph_handle();

//    std::cout << "Test GetOutV" << std::endl;
//    lightgraph::__time_t dep_t = 9, test_time_weight = 2, latest_arr_t = 20;
//    auto iter = graph_handle->GetOutV(1001, "knows", dep_t, test_time_weight, latest_arr_t);
//    lightgraph::__vertex_id_t dst;
//    lightgraph::__time_t arr_time;
//    while(iter->GetNext(dst, arr_time)) {
//        std::cout << "|| Departure -- vertex: 1001, time: " << dep_t << " || "
//                  << "Arrival -- vertex: " << dst << ", time: " << arr_time << " ||" << std::endl;
//    }

    operator_id_t dataflow_id = 0;
    auto* dataflow_addr = new byte_t[sizeof(operator_id_t)];
    memcpy(dataflow_addr, reinterpret_cast<byte_t*>(&dataflow_id), sizeof(operator_id_t));
    dataflow query{nullptr, nullptr, dataflow_addr, sizeof(operator_id_t)};

    light_node init_query_node{1001, 9};
    auto* activating_payload = new message_payload(message_data_type::USER_DATA);
    activating_payload->data.push_back(init_query_node);
    auto* activating_msg = new tag_message(0, message_type::USER);
    activating_msg->payload = activating_payload;
    byte_t activating_msg_addr_len = sizeof(operator_id_t) + sizeof(operator_id_t);
    memcpy(activating_msg->addrs, &activating_msg_addr_len, sizeof(byte_t));
    memcpy(activating_msg->addrs + sizeof(byte_t),
            reinterpret_cast<byte_t*>(&dataflow_id), sizeof(operator_id_t));
    operator_id_t init_op_id = 0;
    memcpy(activating_msg->addrs + sizeof(byte_t) + sizeof(operator_id_t),
            reinterpret_cast<byte_t*>(&init_op_id), sizeof(operator_id_t));
    query.enque_message(activating_msg);

    auto* activating_eos = new tag_message(0, message_type::USER);
    activating_eos->payload = new message_payload{message_data_type::EOS};
    memcpy(activating_eos->addrs, &activating_msg_addr_len, sizeof(byte_t));
    memcpy(activating_eos->addrs + sizeof(byte_t),
           reinterpret_cast<byte_t*>(&dataflow_id), sizeof(operator_id_t));
    memcpy(activating_eos->addrs + sizeof(byte_t) + sizeof(operator_id_t),
           reinterpret_cast<byte_t*>(&init_op_id), sizeof(operator_id_t));
    query.enque_message(activating_eos);

    query.run_and_dispose();

    return 0;
}