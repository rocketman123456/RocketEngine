#pragma once
#include <string>
#include <cstdint>

namespace Rocket {
    namespace details
    {
        template<class>struct hasher;
        template<>
        struct hasher<std::string>
        {
            uint64_t constexpr operator()(char const *input) const { return *input ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) : 5381; }
            uint64_t operator()( const std::string& str ) const { return (*this)(str.c_str()); }
        };

        constexpr int16_t i16(const char *s, int32_t v) { return *s ? i16(s + 1, v * 256 + *s) : v; }
        constexpr int32_t i32(const char *s, int32_t v) { return *s ? i32(s + 1, v * 256 + *s) : v; }
        constexpr int64_t i64(const char *s, int32_t v) { return *s ? i64(s + 1, v * 256 + *s) : v; }
        constexpr uint16_t u16(const char *s, uint16_t v) { return *s ? u16(s + 1, v * 256 + *s) : v; }
        constexpr uint32_t u32(const char *s, uint32_t v) { return *s ? u32(s + 1, v * 256 + *s) : v; }
        constexpr uint64_t u64(const char *s, uint32_t v) { return *s ? u64(s + 1, v * 256 + *s) : v; }
    }
    
    constexpr int16_t operator "" _i16(const char *s, size_t) { return details::i16(s, 0); }
    constexpr int32_t operator "" _i32(const char *s, size_t) { return details::i32(s, 0); }
    constexpr int64_t operator "" _i64(const char *s, size_t) { return details::i64(s, 0); }
    constexpr uint16_t operator "" _u16(const char *s, size_t) { return details::u16(s, 0); }
    constexpr uint32_t operator "" _u32(const char *s, size_t) { return details::u32(s, 0); }
    constexpr uint64_t operator "" _u64(const char *s, size_t) { return details::u64(s, 0); }

    template<typename T>
    constexpr uint64_t hash(T&& t) { return details::hasher< typename std::decay<T>::type >()(std::forward<T>(t)); }
    constexpr uint64_t operator "" _hash(const char* s, size_t) { return details::hasher<std::string>()(s); }
}