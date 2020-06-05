//
// Created by tramboo on 2020/6/4.
//

#include <lightquery/operator_base.hh>

namespace lightquery
{

void operator_base::send_message(tag_message *msg) {
    _dataflow->enque_message(msg);
}

void operator_base::send_eos(std::vector<light_node>&& eos_data) {
    auto* eos_msg = new tag_message(0, message_type::USER);
    auto* eos_payload = new message_payload(message_data_type::EOS, std::move(eos_data));
    eos_msg->payload = eos_payload;
    flush_ds_addr_to_msg(eos_msg);
    _dataflow->enque_message(eos_msg);
}

void operator_base::flush_ds_addr_to_msg(tag_message *msg) {
    assert(_ds_len < max_addr_len);
    memcpy(msg->addrs, &_ds_len, sizeof(byte_t));
    memcpy(msg->addrs + sizeof(byte_t), _ds_addr, _ds_len);
}

}