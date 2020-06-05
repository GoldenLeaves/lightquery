//
// Created by tramboo on 2020/6/2.
//

#include <lightquery/actor_implementation.hh>
#include <iostream>

namespace lightquery
{

void stateless_actor::run_and_dispose() noexcept {
    _execution_count = 0;
    while (!_mailbox.empty() && not_expired()) {
        tag_message* msg = deque_message();
        if (msg->type == message_type::USER) {
            auto result_f = do_work(msg->payload);
            if(result_f == result_flags::CancelMe) {
                _dataflow->handle_child_termination(_address, _addr_len);
            }
        }
        else if (msg->type == message_type::CANCEL_SCOPE) {
            _dataflow->handle_child_termination(_address, _addr_len);
            delete msg;
            break;
        }
        else {
            std::cout << "Receives an undefined system message" << std::endl;
            delete msg;
        }
    }

    // Send terminated response
    if (_terminating) {
        _dataflow->handle_child_terminated(_address, _addr_len);
        delete this;
        return;
    }

    _is_active = false;
    if (! _mailbox.empty()) {
        activate();
    }
}

void stateless_actor::handle_stop_from_parent() {
    if (_terminating) { return; }
    _terminating = true;
    clean_mailbox();
    // Parent may be waiting for its terminated response
    // Activate self to do this
    if (!_is_active /** not in the task queue */) {
        activate();
    }
}

bool stateless_actor::idle() {
    return _mailbox.empty();
}

}