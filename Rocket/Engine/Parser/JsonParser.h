#pragma once
#include <cstdint>
#include <string>
#include <stack>
#include <cassert>
#include <vector>

namespace Rocket {
    enum JsonType : int32_t {
        JSON_NULL = 0,
        JSON_FALSE,
        JSON_TRUE,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT,
    };

    enum JsonParseResult : int32_t {
        JSON_PARSE_OK = 0,
        JSON_PARSE_EXPECT_VALUE,
        JSON_PARSE_INVALID_VALE,
        JSON_PARSE_ROOT_NOT_SINGULAR,
        JSON_PARSE_NUMBER_TOO_BIG,
        JSON_PARSE_MISS_QUOTATION_MARK,
        JSON_PARSE_INVALID_STRING_ESCAPE,
        JSON_PARSE_INVALID_STRING_CHAR,
        JSON_PARSE_INVALID_UNICODE_HEX,
        JSON_PARSE_INVALID_UNICODE_SURROGATE,
    };

    typedef struct JsonValue JsonValue;
    typedef struct JsonMember JsonMember;

    typedef struct JsonValue {
        JsonType type;
        union {
            double value_d;
            bool value_b;
        };
        std::string value_s;
        std::vector<JsonValue> values;
        std::vector<JsonMember> members;
    } JsonValue;

    typedef struct JsonMember {
        std::string key;
        JsonValue value;
    } JsonMember;

    typedef struct JsonContext {
        std::string json = "";
        int32_t current = 0;
        char* stack = nullptr;
        size_t size = 0;
        size_t top = 0;
    } JsonContext;

    // TODO : check int32_t is big enough
    class JsonParser {
    public:
        JsonParser() = default;
        virtual ~JsonParser() = default;

        int32_t Initialize(const std::string& path, const std::string& file_name);
        int32_t Initialize(const std::string& content);
        void Finalize();

        const JsonContext* GetJsonContext() { return &context_; }
        const JsonValue* GetJsonValue() { return &value_; }

        int32_t Parser(JsonContext& c, JsonValue& v);
        inline int32_t GetType(const JsonValue& v) { return v.type; }
        inline double GetNumber(const JsonValue& v) const { assert(v.type == JSON_NUMBER); return v.value_d; }
        inline void SetNumber(JsonValue& v, double d) { assert(v.type == JSON_NUMBER); v.value_d = d; }
        inline bool GetBoolean(const JsonValue& v) const { assert(v.type == JSON_FALSE || v.type == JSON_TRUE); return v.value_b; }
        inline void SetBoolean(JsonValue& v, bool b) { assert(v.type == JSON_FALSE || v.type == JSON_TRUE); v.value_b = b; }
        inline std::string GetString(const JsonValue& v) const { assert(v.type == JSON_STRING); return v.value_s; }
        inline void SetString(JsonValue& v, const std::string& literal) { assert(v.type == JSON_STRING); v.value_s = literal; }
        inline void SetString(JsonValue& v, const std::string& literal, int32_t size) { assert(v.type == JSON_STRING); v.value_s = literal.substr(0, size); }
        inline int32_t GetArraySize(const JsonValue& v) { assert(v.type == JSON_ARRAY); return v.values.size(); }
        inline JsonValue* GetArrayElement(const JsonValue& v, int32_t index) const { assert(index < v.values.size() && v.type == JSON_ARRAY); return const_cast<JsonValue*>(&v.values[index]); }

    private:
        void ParseWhiteSpace(JsonContext& c);
        int32_t ParseLiteral(JsonContext& c, JsonValue& v, const std::string& literal, JsonType type);
        int32_t ParseValue(JsonContext& c, JsonValue& v);
        int32_t ParseNumber(JsonContext& c, JsonValue& v);
        int32_t ParseString(JsonContext& c, JsonValue& v);
        int32_t ParseArray();

        void* ContextPush(JsonContext& c, int32_t size);
        void* ContextPop(JsonContext& c, int32_t size);
        void EncodeUTF8(JsonContext& c, uint32_t u);
        int32_t ParseHex4(const char* p, int32_t& current, uint32_t& u);

    private:
        JsonContext context_;
        JsonValue value_;
    };
}
