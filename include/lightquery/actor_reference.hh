//
// Created by tramboo on 2020/6/1.
//

#include <lightquery/tag_message.hh>

namespace lightquery
{

class actor_reference {
public:
    actor_reference() = default;
    virtual ~actor_reference() = default;

    virtual void handle_child_termination(byte_t *addr, byte_t addr_len) {};
    virtual void handle_child_terminated(byte_t *addr, byte_t addr_len) {};
    virtual void handle_scope_cancellation(byte_t *prefix, byte_t prefix_len) {};
    virtual void enque_message(tag_message* message) = 0;
    virtual void enque_urgent_message(tag_message* message) = 0;
};

}
