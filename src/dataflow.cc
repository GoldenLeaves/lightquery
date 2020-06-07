//
// Created by tramboo on 2020/6/1.
//

#include <lightquery/dataflow.hh>
#include <iostream>

namespace lightquery
{

uint32_t string_to_unsigned_int(std::string str)
{
	uint32_t result(0);
	for (auto i = str.size() - 1; i >= 0; i--)
	{
        uint32_t temp(0), k = str.size() - i - 1;
		if (isdigit(str[i]))
		{
			temp = str[i] - '0';
			while (k--) { temp *= 10; }
			result += temp;
		}
		else break;
	}
	return result;
}

void dataflow::add_task(std::unique_ptr<actor_base>&& t) {
    _task_queue.push_back(std::unique_ptr<actor_base>(t.release()));
    // For distribution and multithread
    // There is a higher level, at which case the dataflow itself is also an actor group
    // activate();
}

/// Urgent tasks are assigned of a high prioroty (IMT_MAX by default) and pushed to the head of
/// actor's _task_queue. So they are always executed before "regular tasks".
/// Example: a child actor may throw exception while running, which inserts an urgent
/// task into the _task_queue of the dataflow it belongs to.
void dataflow::add_urgent_task(std::unique_ptr<actor_base> &&t) {
    _task_queue.push_back(std::unique_ptr<actor_base>(t.release()));
    // For distribution and multithread
    // There is a higher level, at which case the dataflow itself is also an actor group
    // activate();
}

void dataflow::schedule() {
    // TODO
    auto x = _dataflow_id;
    x++;
}

void dataflow::process_mailbox() {
    while (!_mailbox.empty()) {
        auto msg = deque_message();
        switch (msg->type) {
            case message_type::USER : {
                auto child_addr_len = msg->addrs[0];
                byte_t* child_radix_addr = msg->addrs + sizeof(byte_t);
                void* child_ptr = nullptr;
                actor_base* child = nullptr;
                int search = _actor_tree.lookup(child_radix_addr, &child_ptr, child_addr_len);
                switch (search) {
                    case 1 :    // actor exists
                        child = reinterpret_cast<actor_base*>(child_ptr);
                        break;
                    case 0 : {  // actor not exists
                        // TODO: Create the actor
                        auto op_code = _op_codes.get(child_radix_addr, child_addr_len);
                        switch (std::get<0>(*op_code)) {
                            case op_name::OUT : {
                                // std::cout << "Create Out Op!" << std::endl;
                                // TODO: Parse the third parm of op_code ( "label, latest_arr_time")
                                child = new out_op(this, this, child_radix_addr, child_addr_len,
                                        std::get<1>(*op_code), std::get<2>(*op_code));
                                break;
                            }
                            case op_name::LIMIT : {
                                // std::cout << "Create limit Op!" << std::endl;
                                child = new limit_op(this, this, child_radix_addr, child_addr_len,
                                        std::get<1>(*op_code),
                                        string_to_unsigned_int(std::get<2>(*op_code)));
                                break;
                            }
                            default:
                                break;
                        }
                        if (child) {
                            _actor_tree.insert(child_radix_addr, child, child_addr_len);
                        }
                        break;
                    }
                    case -1 :   // actor is disabled
                        continue;
                    default : break;
                }
                msg->type <= message_type::FLOW_CONTROL
                ? child->enque_message(msg) : child->enque_urgent_message(msg);
                break;
            }
            case message_type::CANCEL_SCOPE : {
                auto prefix_length = msg->addrs[0]; /** = scope addr len*/
                if(prefix_length > _addr_len) {
                    // Cancel a scope
                    handle_scope_cancellation(msg->addrs + sizeof(byte_t) /** =scope addr start idx*/, prefix_length);
                }
                else {
                    // Cancel the current actor group
                    // TODO: its parent(thread) needs to cancel this dataflow
                }
                delete msg;
                break;
            }
            default : {
                // TODO: deal with other type of msg
                std::cout << "receives undefined type msg!" << std::endl;
                delete msg;
            }
        }
    }
}

void dataflow::run_and_dispose() noexcept {
    while (true) {
        process_mailbox();
        schedule();
        while (!_task_queue.empty() && not_expired()) {
            auto actor = std::move(_task_queue.front());
            _task_queue.pop_front();
            actor->run_and_dispose();
            actor.release();
        }
        if (_task_queue.empty() && _mailbox.empty()) {
            break;
        }
    }
}

void dataflow::handle_child_termination(byte_t *child_radix_addr, byte_t addr_len) {
    void* child_actor_addr = nullptr;
    auto search = _actor_tree.lookup(child_radix_addr, &child_actor_addr, addr_len);
    if (search == -1) { return; }   // actor disabled
    assert (search  == 1);  // only actor existing in the tree calls this function
    ++_num_terminating_children;
    _actor_tree.disable(child_radix_addr, addr_len);
    auto child = reinterpret_cast<actor_base*>(child_actor_addr);
    child->handle_stop_from_parent();
}

void dataflow::handle_child_terminated(byte_t *child_radix_addr, byte_t addr_len) {
    --_num_terminating_children;
}

void dataflow::handle_scope_cancellation(byte_t *prefix, byte_t prefix_len) {
    auto stop_func = [] (void* data, void* target) mutable { // only non-disabled actor would apply this function.
        reinterpret_cast<dataflow*>(data)->_num_terminating_children += 1;
        reinterpret_cast<actor_base*>(target)->handle_stop_from_parent();
    };
    _actor_tree.iter_prefix_apply(prefix, prefix_len, stop_func, this);
    _actor_tree.disable(prefix, prefix_len);
}

void dataflow::handle_stop_from_parent() {
    if (_terminating) { return; }
    _terminating = true;
    handle_scope_cancellation(_address, _addr_len /** =prefix_len */); // iterate and stop all children in this actor group
    clean_mailbox();
    // For distribution and multithread
    // Parent may be waiting for its terminated response
    // Activate self to do this
    if (!_num_terminating_children && !_is_active) {
        activate();
    }
}

bool dataflow::idle() {
    return _task_queue.empty() && _mailbox.empty();
}

void dataflow::enque_message(tag_message *msg) {
    _mailbox.push_back(msg);
}

void dataflow::enque_urgent_message(tag_message *msg) {
    _mailbox.push_front(msg);
}

}