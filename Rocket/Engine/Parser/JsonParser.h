#pragma once
#include <cstdint>
#include <string>

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
    };

    class JsonParser {
        typedef struct {
            JsonType type;
            union {
                double value;
            };
        } JsonValue;

        typedef struct {
            std::string json;
            int32_t current;
        } JsonContext;
    public:
        JsonParser() = default;
        virtual ~JsonParser() = default;

        int32_t Initialize(const std::string& path, const std::string& file_name);
        int32_t Initialize(const std::string& content);
        void Finalize();

    private:
        int32_t Parser(JsonContext& c, JsonValue& v);
        inline int32_t GetType(const JsonValue& v) { return v.type; }
        inline double GetNumber(JsonValue& v) { return v.value; }

        void ParseWhiteSpace(JsonContext& c);
        int32_t ParseLiteral(JsonContext& c, JsonValue& v, const char* literal, JsonType type);
        int32_t ParseValue(JsonContext& c, JsonValue& v);
        int32_t ParseNumber(JsonContext& c, JsonValue& v);

    private:
        JsonContext content_;
        JsonValue value_;
    };
}
