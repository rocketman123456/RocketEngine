#pragma once
#include <string>
#include <cstdint>

namespace Rocket {
    namespace details {
        constexpr int16_t i16(const char* s, int16_t v) { return *s ? i16(s + 1, v * 256 + *s) : v; }
        constexpr int32_t i32(const char* s, int32_t v) { return *s ? i32(s + 1, v * 256 + *s) : v; }
        constexpr int64_t i64(const char* s, int64_t v) { return *s ? i64(s + 1, v * 256 + *s) : v; }
        constexpr uint16_t u16(const char* s, uint16_t v) { return *s ? u16(s + 1, v * 256 + *s) : v; }
        constexpr uint32_t u32(const char* s, uint32_t v) { return *s ? u32(s + 1, v * 256 + *s) : v; }
        constexpr uint64_t u64(const char* s, uint64_t v) { return *s ? u64(s + 1, v * 256 + *s) : v; }
    }
    
    // Recommand use these
    constexpr int16_t operator "" _i16(const char* s, size_t) { return details::i16(s, 0); }
    constexpr int32_t operator "" _i32(const char* s, size_t) { return details::i32(s, 0); }
    constexpr int64_t operator "" _i64(const char* s, size_t) { return details::i64(s, 0); }
    constexpr uint16_t operator "" _u16(const char* s, size_t) { return details::u16(s, 0); }
    constexpr uint32_t operator "" _u32(const char* s, size_t) { return details::u32(s, 0); }
    constexpr uint64_t operator "" _u64(const char* s, size_t) { return details::u64(s, 0); }

    // Recommand use these
    static inline int16_t hash_i16(const std::string& s) { return details::i16(s.c_str(), 0); }
    static inline int32_t hash_i32(const std::string& s) { return details::i32(s.c_str(), 0); }
    static inline int64_t hash_i64(const std::string& s) { return details::i64(s.c_str(), 0); }
    static inline uint16_t hash_u16(const std::string& s) { return details::u16(s.c_str(), 0); }
    static inline uint32_t hash_u32(const std::string& s) { return details::u32(s.c_str(), 0); }
    static inline uint64_t hash_u64(const std::string& s) { return details::u64(s.c_str(), 0); }

    namespace details {
        template<class>struct hasher;
        template<>
        struct hasher<std::string> {
            uint64_t constexpr operator()(char const* input) const { return *input ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) : 5381; }
            uint64_t operator()( const std::string& str ) const { return (*this)(str.c_str()); }
        };

        template<>
        struct hasher<const char *> {
            uint64_t constexpr operator()(char const* input) const { return *input ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) : 5381; }
            uint64_t operator()( const std::string& str ) const { return (*this)(str.c_str()); }
        };
    }

    // Replacements
    template<typename T>
    constexpr uint64_t hash(T&& t) { return Rocket::details::hasher<typename std::decay<T>::type>()(std::forward<T>(t)); }
    constexpr uint64_t operator "" _hash(const char* s, size_t) { return Rocket::details::hasher<std::string>()(s); }

    // Final Options
    template<typename T, 
        std::enable_if<
            std::is_same<T, int16_t>::value ||
            std::is_same<T, uint16_t>::value ||
            std::is_same<T, int32_t>::value ||
            std::is_same<T, uint32_t>::value ||
            std::is_same<T, int64_t>::value ||
            std::is_same<T, uint64_t>::value
        > 
    >
    T hash_1(const std::string& key) {
        T hash_ = 5381;
        auto count = key.size();
        while(count > 0) {
            hash_ += (hash_ << 5) + key[key.size() - count];
            count--;
        }
        return (hash_ & 0x7fffffff);
    }

    template<typename T, 
        std::enable_if<
            std::is_same<T, int16_t>::value ||
            std::is_same<T, uint16_t>::value ||
            std::is_same<T, int32_t>::value ||
            std::is_same<T, uint32_t>::value ||
            std::is_same<T, int64_t>::value ||
            std::is_same<T, uint64_t>::value
        > 
    >
    T hash_2(const std::string& key) {
        T seed = 131;
        T hash_ = 0;
        std::string str = key + "key2";
        auto count = str.size();
        while(count > 0) {
            hash_ = hash_ * seed + str[str.size() - count];
            count--;
        }
        return (hash_ & 0x7fffffff);
    }

    template<typename T, 
        std::enable_if<
            std::is_same<T, int16_t>::value ||
            std::is_same<T, uint16_t>::value ||
            std::is_same<T, int32_t>::value ||
            std::is_same<T, uint32_t>::value ||
            std::is_same<T, int64_t>::value ||
            std::is_same<T, uint64_t>::value
        > 
    >
    T hash_3(const std::string& key) {
        T hash_ = 0;
        std::string str = key + "keykey3";
        auto count = str.size();
        for(int i = 0; i < count; ++i) {
            if((i * 1) == 0) {
                hash_ ^= ((hash_ << 7) ^ (str[i] ^ hash_ >> 3));
            } else {
                hash_ ^= (~(hash_ << 11) ^ (str[i] ^ hash_ >> 5));
            }
        }
        return (hash_ & 0x7fffffff);
    }

    template<typename T, 
        std::enable_if<
            std::is_same<T, int16_t>::value ||
            std::is_same<T, uint16_t>::value ||
            std::is_same<T, int32_t>::value ||
            std::is_same<T, uint32_t>::value ||
            std::is_same<T, int64_t>::value ||
            std::is_same<T, uint64_t>::value
        > 
    >
    T hash_4(const std::string& key) {
        T hash_ = 5381;
        std::string str = key + "keykeykey4";
        auto count = str.size();
        while(count > 0) {
            hash_ += (hash_ << 5) + str[str.size() - count];
            count--;
        }
        return (hash_ & 0x7fffffff);
    }
}