#include "Parser/ObjParser.h"
#include "FileSystem/FileSystem.h"

namespace Rocket {
    int32_t ObjParser::Initialize() {
        full_path_ = path_ + name_;
        if (path_.substr(path_.size() - 4, 4) != ".obj")
            return 1;

        loaded_meshes.clear();
        loaded_vertices.clear();
        loaded_indices.clear();
        loaded_materials.clear();

        auto file = FileSystem::OpenSync(path_, name_, FileOperateMode::READ_TEXT);
        FileBuffer buffer;
        file->ReadAll(buffer);
        content_ = std::string((char*)buffer.buffer);
        FileSystem::CloseSync(std::move(file));

        return 0;
    }

    void ObjParser::Finalize() {
        loaded_meshes.clear();
        loaded_vertices.clear();
        loaded_indices.clear();
        loaded_materials.clear();
    }

    void ObjParser::Parse() {

    }
}
