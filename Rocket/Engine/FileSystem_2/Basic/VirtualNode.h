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
            std::string path_ = path;
            Replace(path_, "\\", "/");
            SplitName(path_);
            CheckName(file_path, file_name);
            SplitPath();
        }
        explicit VirtualNode(const std::string& path, const std::string& name) {
            std::string path_ = path + name;
            Replace(path_, "\\", "/");
            SplitName(path_);
            CheckName(file_path, file_name);
            SplitPath();
        }
        ~VirtualNode() = default;

        void SplitName(const std::string& path) {
            // for "/User/Name/File.txt"
            // we will get "/User/Name/" as base path,
            // get "File.txt" as file name
            SplitLastSingleChar(path, file_path, file_name, '/');
            if(file_name.length() == 0) {
                RK_ERROR(File, "Invalid File Name {}", path);
                throw std::runtime_error("Invalid File Name");
            }
            if(file_path.length() == 0) {
                file_path = "/";
            }
        }

        void CheckName(std::string& path, std::string& name) {
            if(!EndsWith(path, "/")) { path += "/"; }
            if(StartsWith(name, "/")) { name = name.substr(1, name.length() - 1); }
        }

        void SplitPath() {
            SplitSingleChar(file_path, path_stack, '/');
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
