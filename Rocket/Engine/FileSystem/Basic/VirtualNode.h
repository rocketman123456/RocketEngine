#pragma once
#include "Core/MemoryDefine.h"
#include "Utils/StringUtils.h"
#include "Utils/Hashing.h"
#include "Log/Log.h"

#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace Rocket {
    class VirtualBlock;
    class VirtualFileSystem;
    // Contain Basic and Additional File Infos
    // Can be modified if needed
    class VirtualNode {
    public:
        explicit VirtualNode(const std::string& path) {
            Parse(path);
            CheckName(file_path, file_name);
            Replace(file_path, "\\", "/");
            SplitPath();
        }
        explicit VirtualNode(const std::string& path, const std::string& name) 
            : file_path(path), file_name(name) {
            CheckName(file_path, file_name);
            Replace(file_path, "\\", "/");
            SplitPath();
        }
        ~VirtualNode() = default;
    private:
        void Parse(const std::string& path) {
            // for "/User/Name/File.txt"
            // we will get "/User/Name/" as base path,
            // get "File.txt" as file name
            std::size_t found = path.rfind("/");
            if (found != std::string::npos) {
                file_path = path.substr(0, found + 1);
                if (found != path.length()) {
                    file_name = path.substr(found + 1, path.length() - found - 1);
                } else {
                    RK_ERROR(File, "Invalid File Name {}", path);
                    throw std::runtime_error("Invalid File Name");
                }
            } else {
                file_name = path;
                file_path = "/";
            }
        }

        void CheckName(std::string& path, std::string& name) {
            if(!EndsWith(path, "/")) { path += "/"; }
            if(StartsWith(name, "/")) { name = name.substr(1, name.length() - 1); }
        }

        void SplitPath() {
            SplitSingleChar(file_path, path_stack, '/');
            // for(auto sub_str : path_stack) {
            //     RK_TRACE(File, "{}", sub_str);
            // }
        }
    public:
        std::vector<std::string> path_stack = {};
        std::string file_path = "";  // file basic path in vfs
        std::string file_name = "";  // file name
        std::shared_ptr<VirtualBlock> virtual_block = nullptr;
        std::shared_ptr<VirtualNode> virtual_link = nullptr;
    };

    CLASS_PTR(VirtualNode);
}
