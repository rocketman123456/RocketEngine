#include "Parser/JsonParser.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/OsFile.h"

namespace Rocket {
    int32_t JsonParser::Initialize(const std::string& path, const std::string& file_name) {
        return 0;
    }

    int32_t JsonParser::Initialize(const std::string& content) {
        return 0;
    }

    void JsonParser::Finalize() {
    }

    int32_t JsonParser::Parser(JsonParser::JsonNode* node, const std::string& content) {
        return 0;
    }

    
}
