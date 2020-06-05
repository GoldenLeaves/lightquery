//
// Created by tramboo on 2020/6/2.
//

#include <lightquery/actor_core.hh>

namespace lightquery
{

// adjust the default time slice of each run
//const uint32_t _default_max_concurrency = 10;

class stateless_actor: public actor_base {
//    uint32_t _max_concurrency = _default_max_concurrency;
//    uint32_t _cur_concurrency = 0;
protected:
    enum result_flags : uint8_t {
        Available,
        CancelMe,
        Error,
        // TODO: Other State
    };
public:
    stateless_actor(execution_context_base* exec_ctx, actor_reference* dataflow,
                    byte_t* addr, byte_t addr_len)
            : actor_base(exec_ctx, dataflow, addr, addr_len) {}
    ~stateless_actor() override = default;

    void run_and_dispose() noexcept override;
    void handle_stop_from_parent() override;
    virtual result_flags do_work(message_payload *data) = 0;
    bool idle() override;
};

}
