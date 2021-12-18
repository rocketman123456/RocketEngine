#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "FileSystem/ZipFile/ZipUtils.h"
#include "Utils/StringUtils.h"

#include <filesystem>

namespace Rocket {
    ZipFileSystem::ZipFileSystem(const std::string& real_path) 
        : real_path(real_path), virtual_path("/") {}
    ZipFileSystem::ZipFileSystem(const std::string& real_path, const std::string& virtual_path)
        : real_path(real_path), virtual_path(virtual_path) {}

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
        if(!EndsWith(real_path, "/")) 
            real_path += "/";
        virtual_path = Replace(virtual_path, "\\", "/");
        if(!EndsWith(virtual_path, "/")) 
            virtual_path += "/";
        if(!StartsWith(virtual_path, "/"))
            virtual_path = "/" + virtual_path;
        RK_TRACE(File, "Current path: {}, {}", virtual_path, real_path);
    }

    void ZipFileSystem::CheckFileSystem() {
        std::filesystem::path basic = real_path;
        if(!std::filesystem::exists(basic)) {
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
            if(EndsWith(name, "/")) {
                // Is Dir
                // TODO :
            } else {
                // Is File
                // TODO :
            }
            RK_TRACE(File, "Zip File Name: {}", name);
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

    VNodeList ZipFileSystem::VNodes(const std::string& dir) const {
        VNodeList nodes = {};
        for(auto& node : node_map) {
            nodes.push_back(node.second);
        }
        return nodes;
    }

    VBlockList ZipFileSystem::VBlocks(const std::string& dir) const {
        VBlockList blocks = {};
        for(auto& block : block_map) {
            blocks.push_back(block.second);
        }
        return blocks;
    }

    bool ZipFileSystem::IsFileExists(const std::string& file_path) const {
        auto found = node_map.find(file_path);
        if(found == node_map.end())
            return false;
        else
            return true;
    }

    bool ZipFileSystem::IsDirExists(const std::string& dir_path) const {
        auto found = block_map.find(dir_path);
        if(found == block_map.end())
            return false;
        else
            return true;
    }

    bool ZipFileSystem::IsFile(const std::string& file_path) const {
        return IsFileExists(file_path);
    }

    bool ZipFileSystem::IsDir(const std::string& dir_path) const {
        return IsDirExists(dir_path);
    }

    bool ZipFileSystem::IsReadOnly() const {
        return IsNativeReadOnly(real_path);
    }

    FilePtr ZipFileSystem::OpenFile(const std::string& file_path, int32_t mode) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        return nullptr;
    }

    void ZipFileSystem::CloseFile(const FilePtr& file) {
        file->Close();
    }

    std::size_t ZipFileSystem::ReadFile(const FilePtr& file, FileBuffer* data) {
        return file->Read(data);
    }

    std::size_t ZipFileSystem::WriteFile(FilePtr& file, const FileBuffer& data) {
        return file->Write(data);
    }

    bool ZipFileSystem::CreateFile(const std::string& file_path) {
        RK_WARN(File, "Create File Not Supported");
        return false;
    }

    bool ZipFileSystem::RemoveFile(const std::string& file_path) {
        RK_WARN(File, "Remove File Not Supported");
        return false;
    }

    std::size_t ZipFileSystem::FileSize(const FilePtr& file) const {
        return file->Size();
    }

    bool ZipFileSystem::CreateDir(const std::string& dir_path) {
        RK_WARN(File, "Create Dir Not Supported");
        return false;
    }

    bool ZipFileSystem::RemoveDir(const std::string& dir_path) {
        RK_WARN(File, "Remove Dir Not Supported");
        return false;
    }
}
