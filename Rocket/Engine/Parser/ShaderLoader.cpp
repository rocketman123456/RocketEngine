#include "Parser/ShaderLoader.h"
#include "Log/Log.h"

namespace Rocket {
    std::string ShaderLoader(VirtualFileSystemPtr& vfs, const std::string& name) {
        auto file_ptr = vfs->GetFilePointer(name);
        vfs->OpenFile(file_ptr, FileEnum::READWRITE_TEXT);
        // FileByte buffer_data[] = 
        FileBuffer buffer = {(FileByte*)alloca(file_ptr->Size()+1), file_ptr->Size()+1};
        // FileBuffer buffer = {new FileByte[file_ptr->Size()+1], file_ptr->Size()+1};
        auto result = vfs->ReadFile(file_ptr, &buffer);
        if(result != buffer.size() - 1) {
            RK_ERROR(Graphics, "Loading Include Part Uncomplete: {}", name);
        }
        vfs->CloseFile(file_ptr);
        std::string code(reinterpret_cast<char*>(buffer.data()));
        // delete [] buffer.data();

        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                RK_ERROR(Graphics, "Error while loading shader program: {}", code);
                return std::string();
            }
            const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
            auto include_code = ShaderLoader(vfs, name);
            code.replace(pos, p2-pos+1, include_code.c_str());
        }
        return code;
    }
}