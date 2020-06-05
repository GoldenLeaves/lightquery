//
// Created by tramboo on 2020/6/1.
//

#include <lightquery/actor_reference.hh>
#include <deque>
#include <chrono>

namespace lightquery
{

const std::chrono::microseconds _default_time_slice = std::chrono::microseconds(500);
// The interval (unit: msg/task) that not_expired() calls system_clock::now().
const uint32_t _default_execution_interval = 10;

class execution_context_base;

class actor_base: public actor_reference {
    std::chrono::microseconds _slice = _default_time_slice;
    std::chrono::system_clock::time_point _start_time;
protected:
    bool _terminating = false;
    bool _is_active = false;
    execution_context_base* _exec_ctx;
    actor_reference* _dataflow;
    byte_t* _address;
    byte_t _addr_len;
    uint32_t _execution_count = 0;
    std::deque<tag_message*> _mailbox;

    void activate();
    tag_message* deque_message();
    bool not_expired();
    void clean_mailbox();
public:
    explicit actor_base(execution_context_base* exec_ctx, actor_reference* dataflow,
            byte_t* addr, byte_t addr_len);
    ~actor_base() override { delete[] _address; }

//    virtual actor_reference* _get_actor_internal(tag_message *msg);
//    actor_reference* get_actor(tag_message *msg);
    void enque_message(tag_message* msg) override;
    void enque_urgent_message(tag_message* msg) override;
    void set_time_quota(std::chrono::microseconds slice_ac);
    void set_start_time(std::chrono::system_clock::time_point now_time);
    std::chrono::microseconds get_unused_time(std::chrono::system_clock::time_point now_time);


    virtual void handle_stop_from_parent() = 0;
    virtual bool idle() { return false; }

    virtual void run_and_dispose() noexcept = 0;

};

class execution_context_base {
public:
    execution_context_base() = default;
    virtual ~execution_context_base() = default;

    virtual void add_task(std::unique_ptr<actor_base>&& t) = 0;
    virtual void add_urgent_task(std::unique_ptr<actor_base>&& t) = 0;
};

inline
void actor_base::activate() {
    if (!_is_active) {
        _is_active = true;
        _exec_ctx->add_task(std::unique_ptr<actor_base>(this));
    }
}


} // end namespace lightquery
