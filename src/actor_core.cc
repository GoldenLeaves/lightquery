//
// Created by tramboo on 2020/6/1.
//

#include <lightquery/actor_core.hh>
#include <iostream>

namespace lightquery
{

actor_base::actor_base(execution_context_base *exec_ctx, actor_reference *dataflow,
        byte_t *addr, byte_t addr_len)
    : _exec_ctx(exec_ctx), _dataflow(dataflow)
    , _address(new byte_t[addr_len]), _addr_len(addr_len)
{
    memcpy(_address, addr, addr_len);
}

void actor_base::enque_message(tag_message* message) {
    // std::cout << "Enqueue message in actor!" << std::endl;
    _mailbox.push_back(message);
    activate();
}

void actor_base::enque_urgent_message(tag_message* message) {
    _mailbox.push_front(message);
    activate();
}

tag_message* actor_base::deque_message() {
    auto message = _mailbox.front();
    _mailbox.pop_front();
    return message;
}

void actor_base::set_time_quota(std::chrono::microseconds slice_ac) {
    _slice = slice_ac;
}

void actor_base::set_start_time(std::chrono::system_clock::time_point now_time) {
    _start_time = now_time;
}

bool actor_base::not_expired() {
    if(++_execution_count < _default_execution_interval) {
        return true;
    }
    _execution_count = 0;
    return std::chrono::system_clock::now() - _start_time < _slice;
}

std::chrono::microseconds actor_base::get_unused_time(std::chrono::system_clock::time_point now_time) {
    return _slice - std::chrono::duration_cast<std::chrono::microseconds>(now_time - _start_time);
}

void actor_base::clean_mailbox() {
    while (!_mailbox.empty()) {
        tag_message* msg = deque_message();
        delete msg;
    }
}

}