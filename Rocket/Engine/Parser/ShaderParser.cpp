#include "Parser/ShaderParser.h"
#include "Log/Log.h"

#include <set>

namespace Rocket {
    static std::set<std::string> include_files = {};

    std::string ReadShaderFile(const std::string& root, const std::string& name, uint32_t level) {
        // TODO : use resource cache to load files
        auto full_name = root + '/' + name;
        FILE* file = fopen(full_name.c_str(), "r");
        if (file == nullptr) {
            RK_ERROR(Graphics, "I/O error. Cannot open shader file: {}", full_name);
            return std::string();
        }
        fseek(file, 0L, SEEK_END);
        const auto bytesinfile = ftell(file);
        fseek(file, 0L, SEEK_SET);
        // Read File Buffer
        char* buffer = (char*)alloca(bytesinfile + 1);
        const size_t bytesread = fread(buffer, 1, bytesinfile, file);
        fclose(file);
        buffer[bytesread] = 0;
        std::string code(buffer);
        // Handle Shader Content
        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                RK_ERROR(Graphics, "Error while loading shader program: {}", code);
                return std::string();
            }
            const std::string include_name = code.substr(p1 + 1, p2 - p1 - 1);
            std::string include = "";
            if(include_files.find(include_name) != include_files.end()) {
                RK_WARN(Graphics, "Find Recursive Include File");
            } else {
                include_files.insert(include_name);
                include = ReadShaderFile(root, include_name, level + 1);
            }
            code.replace(pos, p2-pos+1, include.c_str());
        }
        return code;
    }

    std::string ReadShaderFile(const std::string& root, const std::string& name) {
        include_files.clear();
        auto code = ReadShaderFile(root, name, 0);
        include_files.clear();
        return code;
    }

    std::string ReadShaderFile(const std::string& name, VirtualFileSystemPtr vfs, uint32_t level) {
        // TODO : use resource cache to load files
        auto file_ptr = vfs->GetFilePointer(name);
        vfs->OpenFile(file_ptr, FileEnum::READWRITE_TEXT);
        FileBuffer buffer = {new FileByte[file_ptr->Size()+1], file_ptr->Size()+1};
        auto result = vfs->ReadFile(file_ptr, &buffer);
        // assert(result == buffer.size() - 1);
        vfs->CloseFile(file_ptr);
        std::string code(reinterpret_cast<char*>(buffer.data()));
        // Handle Shader Content
        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                RK_ERROR(Graphics, "Error while loading shader program: {}", code);
                return std::string();
            }
            const std::string include_name = code.substr(p1 + 1, p2 - p1 - 1);
            std::string include = "";
            if(include_files.find(include_name) != include_files.end()) {
                RK_WARN(Graphics, "Find Recursive Include File");
            } else {
                include_files.insert(include_name);
                include = ReadShaderFile(include_name, vfs, level + 1);
            }
            code.replace(pos, p2-pos+1, include.c_str());
        }
        return code;
    }

    std::string ReadShaderFile(const std::string& name, VirtualFileSystemPtr vfs) {
        include_files.clear();
        auto code = ReadShaderFile(name, vfs, 0);
        include_files.clear();
        return code;
    }
}