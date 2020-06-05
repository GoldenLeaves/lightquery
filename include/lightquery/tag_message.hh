//
// Created by tramboo on 2020/6/1.
//

#include <lightquery/typedef.hh>
#include <vector>

namespace lightquery
{

enum message_type : uint8_t {
    USER = 0,
    FLOW_CONTROL = 1,
    CREATE_CHILD = 2,
    CANCEL_SCOPE = 3,
    CONFIGURATION = 4,
    ACKNOWLEDGE = 5,
};

enum message_data_type : uint8_t {
    USER_DATA = 0,
    EOS = 1,
    NOTHING = 2,
};

const size_t max_payload_capacity = 100;
struct message_payload {
    message_data_type type;
    std::vector<light_node> data;

    message_payload(): type(message_data_type::NOTHING), data{} {}
    explicit message_payload(message_data_type payload_type): type(payload_type), data{} {}
    message_payload(message_data_type payload_type, std::vector<light_node>&& payload_data)
        : type(payload_type), data(std::move(payload_data)) {}
    ~message_payload() = default;
};

const uint32_t max_addr_len = 19;
const size_t operator_field_len = sizeof(operator_id_t);
const size_t branch_field_len = sizeof(branch_id_t);
struct tag_message {
    uint32_t shard_id = 0;
    message_type type;
    // Address format
    // # TotalLength(1 byte) # DataflowID(2 byte)
    // # (ActorGroupID(2 byte) # BranchID(4 byte)) # ... # (ActorGroupID # BranchID)
    // # ActorID(2 byte)
    byte_t addrs[max_addr_len];
    message_payload* payload;

    explicit tag_message(uint32_t shard_id, message_type type)
        : shard_id(shard_id), type(type), addrs{}, payload{nullptr} {}
    tag_message(const tag_message& other) = delete;
    tag_message(tag_message&& other) noexcept : shard_id(0)
            , type(message_type::USER), addrs{}, payload{nullptr} {
        *this = std::move(other);
    }
    tag_message& operator=(tag_message&& other) noexcept {
        if (this != &other) {
            shard_id = other.shard_id;
            type = other.type;
            memcpy(addrs, other.addrs, max_addr_len);
            payload = other.payload;
        }
        return *this;
    }
    ~tag_message() = default;
};

} // end namespace lightquery
