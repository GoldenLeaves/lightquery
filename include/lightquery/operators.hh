//
// Created by tramboo on 2020/6/4.
//

#pragma once

#include <lightquery/operator_base.hh>
#include <iostream>
#include <utility>

namespace lightquery
{

const time_t time_weight = 2;

class out_op : public operator_base {
    std::string _out_label;
    message_payload* _out_payload;
    time_t _latest_arr_time;
public:
    out_op(execution_context_base* exec_ctx, actor_reference* dataflow, byte_t* addr,
            byte_t addr_len, operator_id_t downstream_op_id, std::string out_label,
            const time_t latest_arrival_time = std::numeric_limits<time_t>::max())
       : operator_base(exec_ctx, dataflow, addr, addr_len, downstream_op_id, addr_len)
       , _out_label(std::move(out_label)), _latest_arr_time(latest_arrival_time)
    {
        // std::cout << "Downstream ID: " << downstream_op_id << std::endl;
        _out_payload = new message_payload(message_data_type::USER_DATA);
        _out_payload->data.reserve(max_payload_capacity);
    }
    ~out_op() override { delete _out_payload; }
    result_flags do_work(message_payload *data) override;
private:
    void flush_out_payload();
};

class limit_op : public operator_base {
    uint32_t _limit_cnt;
    uint32_t _cur_cnt;
public:
    limit_op(execution_context_base* exec_ctx, actor_reference* dataflow, byte_t* addr,
            byte_t addr_len, operator_id_t downstream_op_id, uint32_t limit_count)
       : operator_base(exec_ctx, dataflow, addr, addr_len, downstream_op_id, addr_len)
       , _limit_cnt(limit_count), _cur_cnt(0) {}
    ~limit_op() override = default;
    result_flags do_work(message_payload *data) override;
};

}
