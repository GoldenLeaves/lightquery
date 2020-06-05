//
// Created by tramboo on 2020/6/3.
//

#include <cstdint>

namespace lightquery
{

using byte_t = uint8_t;

inline unsigned* as_u32_ptr(void* data) {
    return reinterpret_cast<uint32_t*>(data);
}

inline unsigned* to_u32_ptr(void* data) {
    return static_cast<uint32_t*>(data);
}

inline byte_t* as_byte_ptr(void* data) {
    return reinterpret_cast<byte_t*>(data);
}

inline byte_t* to_byte_ptr(void* data) {
    return static_cast<byte_t*>(data);
}

}
