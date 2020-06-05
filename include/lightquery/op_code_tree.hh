//
// Created by tramboo on 2020/6/3.
//

#include <lightquery/typedef.hh>
#include <cstring>

namespace lightquery
{

class op_code_tree {
    struct node {
        const uint32_t ignore_len;
        std::vector<node> next;
        code_t *data_ptr;
        node(uint32_t ig_len, std::vector<node> &&next, code_t *d_ptr)
                : ignore_len(ig_len), next(std::move(next)), data_ptr(d_ptr) {}
    };
    node _head;
public:
    op_code_tree() : _head(0, std::vector<node>{}, nullptr) {
        dummy_insert_simple();
    }
    ~op_code_tree() { destruct(_head); }

    static inline code_t *
    gen_code(const op_name op, const operator_id_t ds_id, std::string &&params) {
        return new code_t{op, ds_id, std::move(params)};
    }

    code_t* get(const byte_t *addr, unsigned len) {
        auto* head = &_head;
        while (!head->next.empty()) {
            head = &(head->next[*addr]);
            addr += sizeof(operator_id_t) + head->ignore_len;
        }
        return head->data_ptr;
    }

private:
    static void destruct(node &head) {
        if (head.next.empty() && head.data_ptr) {
            delete head.data_ptr;
        } else {
            for (auto& cn : head.next) {
                destruct(cn);
            }
        }
    }

    void dummy_insert_simple() {
        auto code_get_v = gen_code(op_name::GET_VERTEX, 1, "519402");
        _head.next.emplace_back(node{0, std::vector<node>{}, code_get_v});
        auto code_out = gen_code(op_name::OUT, 2, "knows");
        _head.next.emplace_back(node{0, std::vector<node>{}, code_out});
        auto code_limit = gen_code(op_name::LIMIT, -1, "5");
        _head.next.emplace_back(node{0, std::vector<node>{}, code_limit});
    }

};

}
