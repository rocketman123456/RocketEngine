#pragma once
#include <cstdint>
#include <string>

namespace Rocket {
    enum class JsonType : int32_t {
        JSON_NULL = 0,
        JSON_FALSE,
        JSON_TRUE,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT,
    };

    enum class JsonParseResult : int32_t {
        PARSE_OK = 0,
        PARSE_EXPECT_VALUE,
        PARSE_INVALID_VALE,
        PARSE_ROOT_NOT_SINGULAR,
    };

    class JsonParser {
        typedef struct {
            JsonType type;
        } JsonNode;
    public:
        JsonParser() = default;
        virtual ~JsonParser() = default;

        int32_t Initialize(const std::string& path, const std::string& file_name);
        int32_t Initialize(const std::string& content);
        void Finalize();

    private:
        int32_t Parser(JsonNode* node, const char* content);
        JsonType GetType(const JsonNode* node);

    private:
        std::string content_;
        JsonNode root_;
    };
}
