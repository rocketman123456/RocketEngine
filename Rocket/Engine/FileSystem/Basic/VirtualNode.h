#pragma once
#include "Core/MemoryDefine.h"
#include "Utils/StringUtils.h"
#include "Utils/Hashing.h"
#include "Log/Log.h"

#include <memory>
#include <string>
#include <exception>
#include <stdexcept>

namespace Rocket {
    class VirtualBlock;
    class VirtualNode {
    public:
        explicit VirtualNode(const std::string& path) {
            Parse(path, file_path, file_name);
            CheckName(file_path, file_name);
        }
        explicit VirtualNode(const std::string& path, const std::string& name) : file_path(path), file_name(name) {
            CheckName(file_path, file_name);
        }
        ~VirtualNode() = default;
    private:
        void Parse(const std::string& path, std::string& filepath, std::string& filename) {
            std::size_t found = path.rfind("/");
            if (found != std::string::npos) {
                const std::string filepath = path.substr(0, found + 1);
                if (found != path.length()) {
                    filename = path.substr(found + 1, path.length() - found - 1);
                } else {
                    RK_ERROR(File, "Invalid File Name {}", path);
                    throw std::runtime_error("Invalid File Name");
                }
            } else {
                filename = path;
                filepath = "/";
            }
        }

        void CheckName(std::string& path, std::string& name) {
            if(!EndsWith(path, "/")) { path += "/"; }
            if(StartsWith(name, "/")) { path = name.substr(1, name.length() - 1); }
        }
    private:
        std::string file_path = "";  // file basic path in vfs
        std::string file_name = "";  // file name
        // Tells how to operate file
        std::shared_ptr<VirtualBlock> virtual_block = nullptr;
        std::shared_ptr<VirtualNode> virtual_link = nullptr;
    };
}
