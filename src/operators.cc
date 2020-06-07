//
// Created by tramboo on 2020/6/5.
//

#include <lightquery/operators.hh>
#include <iostream>

namespace lightquery
{

stateless_actor::result_flags lightquery::out_op::do_work(message_payload *data) {
    stateless_actor::result_flags result_f = stateless_actor::result_flags::Available;
    if (data->type == message_data_type::USER_DATA) {
        vertex_id_t dst;
        time_t arr_time;
        for(auto& light_node: data->data) {
//            std::cout << "src vertex: " << light_node.vertex
//                      << " , time: " << light_node.time << std::endl;
            auto iter = graph_handle->GetOutV(
                    light_node.vertex, _out_label, light_node.time, time_weight, _latest_arr_time);
            while(iter->GetNext(dst, arr_time)) {
                _out_payload->data.emplace_back(dst, arr_time);
                if(_out_payload->data.size() >= max_payload_capacity) {
                    flush_out_payload();
                }
            }
        }
    }
    else if (data->type == message_data_type::EOS) {
        if(++_num_rcv_eos == _num_upstreams) {
            // flush the remaining out payload
            flush_out_payload();
            send_eos(std::vector<light_node>{});
            result_f = stateless_actor::result_flags::CancelMe;
        }
    }
    else {
        std::cout << "Received undefined message data type!" << std::endl;
        result_f = stateless_actor::result_flags::Error;
    }

    delete data;
    return result_f;
}

void out_op::flush_out_payload() {
    auto* out_msg = new tag_message(0, message_type::USER);
    out_msg->payload = _out_payload;
    _out_payload = new message_payload(message_data_type::USER_DATA);
    _out_payload->data.reserve(max_payload_capacity);
    flush_ds_addr_to_msg(out_msg);
    send_message(out_msg);
}

stateless_actor::result_flags limit_op::do_work(message_payload *data) {
    stateless_actor::result_flags result_f = stateless_actor::result_flags::Available;
    if(_cur_cnt >= _limit_cnt) { return result_f; }
    if (data->type == message_data_type::USER_DATA) {
        for(auto& light_node: data->data) {
            std::cout << "Vertex: " << light_node.vertex
                      << " || Time: " << light_node.time << std::endl;
            if (++_cur_cnt >= _limit_cnt) {
                result_f = stateless_actor::result_flags::CancelMe;
                break;
            }
        }
    }
    else if (data->type == message_data_type::EOS) {
        if(++_num_rcv_eos == _num_upstreams && _last_op) {
            result_f = stateless_actor::result_flags::CancelMe;
        }
    }
    else {
        std::cout << "Received undefined message data type!" << std::endl;
        result_f = stateless_actor::result_flags::Error;
    }
    delete data;
    return result_f;
}
}


