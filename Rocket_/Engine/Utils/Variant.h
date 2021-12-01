#pragma once
#include <cstdint>

namespace Rocket {
    using string_id = uint64_t;
    struct Variant {
        enum Type {
            TYPE_INT32 = 0,
            TYPE_UINT32,
            TYPE_FLOAT,
            TYPE_DOUBLE,
            TYPE_BOOL,
            TYPE_POINTER,
            TYPE_STRING_ID,
            TYPE_COUNT,
        };

        explicit Variant() : type(Variant::TYPE_INT32), as_int32(0) {}
        explicit Variant(Type _type, string_id data) : type(_type), as_string_id(data) {}
        explicit Variant(Type _type, int32_t data) : type(_type), as_int32(data) {}
        explicit Variant(Type _type, uint32_t data) : type(_type), as_uint32(data) {}
        explicit Variant(Type _type, float data) : type(_type), as_float(data) {}
        explicit Variant(Type _type, double data) : type(_type), as_double(data) {}
        explicit Variant(Type _type, bool data) : type(_type), as_bool(data) {}
        explicit Variant(Type _type, void* data) : type(_type), as_pointer(data) {}
        
        Type type;
        union {
            bool        as_bool;
            int32_t     as_int32;
            uint32_t    as_uint32;
            float       as_float;
            double      as_double;
            void*       as_pointer;
            string_id   as_string_id;
        };
    };
}
