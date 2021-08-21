#include "Parser/JsonParser.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/OsFile.h"

#include <stdlib.h>

namespace Rocket {
    int32_t JsonParser::Initialize(const std::string& path, const std::string& file_name) {
        auto file = FileSystem::OpenSync(path, file_name, FileOperateMode::ReadText);
        FileBuffer buffer;
        file->ReadAll(buffer);
        content_ = std::string((char*)buffer.buffer);
        delete[] buffer.buffer;
        FileSystem::CloseSync(std::move(file));

        Serialize(content_);
        return 0;
    }

    int32_t JsonParser::Initialize(const std::string& content) {
        content_ = content;
        Serialize(content_);
        return 0;
    }

    void JsonParser::Finalize() {
    }
}
