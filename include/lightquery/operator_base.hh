//
// Created by tramboo on 2020/6/4.
//

#include <lightquery/actor_implementation.hh>
#include <lightquery/graph_handle.hh>

namespace lightquery
{

class operator_base: public stateless_actor {
protected:
    byte_t* _ds_addr;
    byte_t _ds_len;
    // current received eos from upstream
    uint32_t _num_rcv_eos;
    // upstream numbers
    // for single thread: upstream numbers = 1.
    // for multiple thread: upstream numbers = thread numbers(On all machines when distributed).
    uint32_t _num_upstreams;
    bool _last_op;
public:
    operator_base(execution_context_base* exec_ctx, actor_reference* dataflow,
            byte_t* addr, byte_t addr_len, operator_id_t downstream_op_id,
            byte_t downstream_len, const uint32_t num_upstreams = shard_number)
        : stateless_actor(exec_ctx, dataflow, addr, addr_len)
        , _ds_addr(new byte_t[downstream_len]), _ds_len(downstream_len)
        , _num_rcv_eos(0), _num_upstreams(num_upstreams)
    {
        if(downstream_op_id == -1) {
            _last_op = true;
            return;
        }
        _last_op = false;
        auto scope_ds_len = std::min(addr_len, downstream_len);
        assert(scope_ds_len >= sizeof(operator_id_t));
        memcpy(_ds_addr, addr, scope_ds_len - sizeof(operator_id_t));
        memcpy(_ds_addr + scope_ds_len - sizeof(operator_id_t),
                reinterpret_cast<byte_t*>(&downstream_op_id),
                sizeof(operator_id_t));
    }
    ~operator_base() override { delete[] _ds_addr; }

    virtual void send_message(tag_message* msg);
    virtual void send_eos(std::vector<light_node>&& eos_data);
protected:
    void flush_ds_addr_to_msg(tag_message* msg);
};

}