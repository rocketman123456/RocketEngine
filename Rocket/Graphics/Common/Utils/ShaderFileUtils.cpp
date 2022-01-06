#include "Common/Utils/ShaderFileUtils.h"
#include "Log/Log.h"

namespace Rocket {
    std::string ReadShaderFile(const std::string& name, VirtualFileSystemPtr vfs) {
        auto file_ptr = vfs->GetFilePointer(name);
        vfs->OpenFile(file_ptr, FileEnum::READWRITE_TEXT);
        FileBuffer buffer = {new FileByte[file_ptr->Size()+1], file_ptr->Size()+1};
        auto result = vfs->ReadFile(file_ptr, &buffer);
        assert(result == buffer.size() - 1);
        vfs->CloseFile(file_ptr);
        std::string code(reinterpret_cast<char*>(buffer.data()));

        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                RK_ERROR(Graphics, "Error while loading shader program: {}", code);
                return std::string();
            }
            const std::string include_name = code.substr(p1 + 1, p2 - p1 - 1);
            const std::string include = ReadShaderFile(include_name, vfs);
            code.replace(pos, p2-pos+1, include.c_str());
        }
        return code;
    }
}