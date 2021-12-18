#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "FileSystem/ZipFile/ZipUtils.h"
#include "Utils/StringUtils.h"

#include <filesystem>
#include <memory>

namespace Rocket {
    ZipFileSystem::ZipFileSystem(const std::string& real_path) 
        : FileSystem(real_path, "/") {}
    ZipFileSystem::ZipFileSystem(const std::string& real_path, const std::string& virtual_path)
        : FileSystem(real_path, virtual_path) {}

    void ZipFileSystem::Initialize() {
        if(IsInitialized()) {
            RK_INFO(File, "File System Already Initialized");
            return;
        }

        NormalizePath();
        CheckFileSystem();

        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        GetRootName();

        // Open Zip
        zip_archive = OpenZip(real_path, ZIP_CREATE);
        if(zip_archive == nullptr) {
            throw std::runtime_error("Unable to Open Zip");
        }

        BuildVirtualSystem();

        is_initialized = true;
    }

    void ZipFileSystem::NormalizePath() {
        real_path = Replace(real_path, "\\", "/");
        virtual_path = Replace(virtual_path, "\\", "/");
        if(!EndsWith(virtual_path, "/")) 
            virtual_path += "/";
        if(!StartsWith(virtual_path, "/"))
            virtual_path = "/" + virtual_path;
        RK_TRACE(File, "Current path: {}, {}", virtual_path, real_path);
    }

    void ZipFileSystem::CheckFileSystem() {
        std::filesystem::path basic = real_path;
        if(!std::filesystem::is_regular_file(basic)) {
            RK_TRACE(File, "Zip File System {} Not Exist", real_path);
            throw std::invalid_argument("Zip File System Not Exist");
        }
    }

    void ZipFileSystem::GetRootName() {
        std::vector<std::string> dir_stack;
        SplitSingleChar(virtual_path, &dir_stack, '/');
        if(dir_stack.size() > 0) {
            root->name = dir_stack.at(dir_stack.size() - 1);
        } else {
            root->name = "/";
        }
    }

    void ZipFileSystem::BuildVirtualSystem() {
        // Iterate Through Zip Files
        zip_int64_t num_entries = zip_get_num_entries(zip_archive, 0);
        for(zip_int64_t i = 0; i < num_entries; ++i) {
            const char* name = zip_get_name(zip_archive, i, 0);
            std::vector<std::string> dir_stack;
            SplitSingleChar(name, &dir_stack, '/');
            if(EndsWith(name, "/")) {
                // Is Dir
                VirtualBlockPtr block = CreateVirtualBlock(root, dir_stack, 0);
                //block_map[virtual_path + name] = block;
            } else {
                // Is File
                VirtualNodePtr node = CreateVirtualNode(root, dir_stack);
                //node_map[virtual_path + name] = node;
            }
            // RK_TRACE(File, "Zip File Name: {}", name);
        }
    }

    VirtualBlockPtr ZipFileSystem::CreateVirtualBlock(VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        if(root == nullptr) return nullptr;
        if(dirs.size() == 0 || level == dirs.size()) return root;
        auto found = root->block_map.find(dirs[level]);
        if(found == root->block_map.end()) {
            VirtualBlockPtr block = std::make_shared<VirtualBlock>();
            block->parent = root;
            block->name = dirs[level];
            block->path = root->path + block->name + "/";
            root->block_map[block->name] = block;
            block_map[block->path] = block;
            RK_TRACE(File, "Block Path: {}", block->path);
            return CreateVirtualBlock(block, dirs, level + 1);
        } else {
            return CreateVirtualBlock(found->second, dirs, level + 1);
        }
    }

    VirtualNodePtr ZipFileSystem::CreateVirtualNode(VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        if(root == nullptr || dirs.size() == 0) return nullptr;
        std::vector<std::string> dirs_copy;
        for(int i = 0; i < dirs.size() - 1; ++i) {
            dirs_copy.push_back(dirs[i]);
        }
        auto block = CreateVirtualBlock(root, dirs_copy, 0);

        auto found = block->node_map.find(dirs[0]);
        if(found == block->node_map.end()) {
            auto node = std::make_shared<VirtualNode>();
            node->vblock = block;
            node->path = block->path;
            node->name = dirs[dirs.size() - 1];
            block->node_map[node->name] = node;
            node_map[node->path + node->name] = node;
            RK_TRACE(File, "Node Path: {}", node->path + node->name);
            return node;
        } else {
            return found->second;
        }
    }

    void ZipFileSystem::Finalize() {
        CloseZip(zip_archive);
        root = nullptr;
        real_path = "";
        virtual_path = "";
        node_map.clear();
        block_map.clear();
        is_initialized = false;
    }

    void ZipFileSystem::SetVirtualPath(const std::string& vpath) {
        virtual_path = vpath;
        if(!IsInitialized()) { return; }

        // Normalize Path
        NormalizePath();
        // Init Root Block
        root = nullptr;
        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    void ZipFileSystem::SetRealPath(const std::string& rpath) {
        if(IsInitialized()) {
            RK_WARN(File, "Zip File System Not Able To Change Real Path After Initialize");
        } else {
            real_path = rpath;
        }
    }

    bool ZipFileSystem::IsReadOnly() const {
        return IsNativeReadOnly(real_path);
    }

    FilePtr ZipFileSystem::GetFilePointer(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        auto full_path = file_path.substr(virtual_path.size());
        auto file = std::make_shared<ZipFile>(full_path, file_path, zip_archive);
        return file;
    }
}
