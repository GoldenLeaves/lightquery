//
// Created by tramboo on 2020/5/31.
//

#include <lightquery/operators.hh>
#include <lightquery/op_code_tree.hh>
#include <lightquery/util/radix_tree_index.hh>
#include <memory>

namespace lightquery
{

class dataflow: public actor_base, public execution_context_base {
    op_code_tree _op_codes;
    const uint32_t _dataflow_id = 0;
    unsigned long _num_terminating_children = 0;
    radix_tree_index<actor_base*> _actor_tree;
    std::deque<std::unique_ptr<actor_base>> _task_queue;
public:
    dataflow(execution_context_base* exec_ctx, actor_reference* parent, byte_t* addr, byte_t addr_len)
        : actor_base(exec_ctx, parent, addr, addr_len)
        , _op_codes(op_code_tree()) {}
    ~dataflow() override = default;

    void add_task(std::unique_ptr<actor_base>&& t) override;
    void add_urgent_task(std::unique_ptr<actor_base>&& t) override;
    void run_and_dispose() noexcept override;
    void handle_stop_from_parent() override;
    void handle_child_termination(byte_t* child_radix_addr, byte_t addr_len) override;
    void handle_child_terminated(byte_t* child_radix_addr, byte_t addr_len) override;
    bool idle() override;
private:
    void schedule();
    void process_mailbox();
    void handle_scope_cancellation(byte_t *prefix, byte_t prefix_len) override;
};

} // end namespace lightquery
