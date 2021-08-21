#pragma once
#include <nlohmann/json.hpp>

#include <string>
#include <memory>
#include <cstdint>

namespace Rocket {
    class JsonParser {
    public:
        JsonParser() = default;
        virtual ~JsonParser() = default;

        int32_t Initialize(const std::string& path, const std::string& file_name);
        int32_t Initialize(const std::string& content);
        void Finalize();

        inline void Serialize(const std::string& content) { json_ = nlohmann::json::parse(content); }
        inline std::string Deserialize() { return json_.dump(); }

        nlohmann::json* GetRawJson() { return &json_; }

    private:
        std::string content_;
        nlohmann::json json_;
    };

    using JsonParserPtr = std::unique_ptr<JsonParser>;
}