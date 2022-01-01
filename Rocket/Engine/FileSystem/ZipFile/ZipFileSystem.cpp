#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "FileSystem/ZipFile/ZipUtils.h"
#include "Log/Instrumentor.h"
#include "Utils/StringUtils.h"

#include <filesystem>
#include <memory>

namespace Rocket {
    ZipFileSystem::ZipFileSystem(const std::string& real_path) 
        : FileSystem(real_path, "/") {}
    ZipFileSystem::ZipFileSystem(const std::string& real_path, const std::string& virtual_path)
        : FileSystem(real_path, virtual_path) {}

    void ZipFileSystem::Initialize() {
        RK_PROFILE_FUNCTION();
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
        {
            zip_archive = OpenZip(real_path, ZIP_CREATE);
            if(zip_archive == nullptr) {
                throw std::runtime_error("Unable to Open Zip");
            }
        }
        BuildVirtualSystem();
        is_initialized = true;
    }

    void ZipFileSystem::NormalizePath() {
        RK_PROFILE_FUNCTION();
        real_path = Replace(real_path, "\\", "/");
        virtual_path = Replace(virtual_path, "\\", "/");
        if(!EndsWith(virtual_path, "/")) 
            virtual_path += "/";
        if(!StartsWith(virtual_path, "/"))
            virtual_path = "/" + virtual_path;
        RK_TRACE(File, "Current path: {}, {}", virtual_path, real_path);
    }

    void ZipFileSystem::CheckFileSystem() {
        RK_PROFILE_FUNCTION();
        std::filesystem::path basic = real_path;
        if(!std::filesystem::is_regular_file(basic)) {
            RK_TRACE(File, "Zip File System {} Not Exist", real_path);
            throw std::invalid_argument("Zip File System Not Exist");
        }
    }

    void ZipFileSystem::GetRootName() {
        std::string dir;
        RK_PROFILE_FUNCTION();
        std::string file_name;
        SplitLastSingleChar(virtual_path, &dir, &file_name, '/');
        SplitLastSingleChar(dir, nullptr, &file_name, '/');
        if(file_name.length() > 0) {
            root->name = file_name;
        } else {
            root->name = "/";
        }
    }

    void ZipFileSystem::BuildVirtualSystem() {
        RK_PROFILE_FUNCTION();
        // Iterate Through Zip Files
        zip_int64_t num_entries = zip_get_num_entries(zip_archive, 0);
        for(zip_int64_t i = 0; i < num_entries; ++i) {
            const char* name = zip_get_name(zip_archive, i, 0);
            if(EndsWith(name, "/")) { // Is Dir
                VirtualBlockPtr block = CreateVirtualBlock(root, name);
            } else { // Is File
                VirtualNodePtr node = CreateVirtualNode(root, name);
            }
        }
    }

    void ZipFileSystem::Finalize() {
        CloseZip(zip_archive);
        root = nullptr;
        real_path = "";
        virtual_path = "";
        is_initialized = false;
    }

    void ZipFileSystem::SetVirtualPath(const std::string& vpath) {
        RK_PROFILE_FUNCTION();
        virtual_path = vpath;
        if(!IsInitialized()) { return; }
        // Normalize Path
        NormalizePath();
        // Init Root Block
        root->path = virtual_path;
        root->node_map.clear();
        root->block_map.clear();
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    void ZipFileSystem::SetRealPath(const std::string& rpath) {
        RK_PROFILE_FUNCTION();
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
        RK_PROFILE_FUNCTION();
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        auto full_path = file_path.substr(virtual_path.size());
        auto file = std::make_shared<ZipFile>(full_path, file_path, zip_archive);
        return file;
    }

    bool ZipFileSystem::CreateFile(const std::string& file_path) {
        if(IsFileExists(file_path)) {
            RK_WARN(File, "File Existed {}", file_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Create File in Real File System
        const char buf[]="test";
        zip_source_t* source = zip_source_buffer(zip_archive, buf, sizeof(buf), 0);
        auto result = zip_file_add(zip_archive, file_path.c_str(), source, ZIP_FL_ENC_UTF_8);
        if (source == nullptr || result < 0) {
            zip_source_free(source);
            RK_WARN(File, "error adding file: {}", zip_strerror(zip_archive));
        }
        // Add info to VFS
        auto final_block = CreateVirtualBlock(root, dir);
        // Add info to VFS
        auto node = std::make_shared<VirtualNode>();
        node->name = file_name;
        node->path = final_block->path;
        node->vblock = final_block;
        final_block->node_map[file_name] = node;
        return true;
    }

    bool ZipFileSystem::RemoveFile(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Existed {}", file_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Remove File in Real File System
        auto file_index = zip_name_locate(zip_archive, sub_path.c_str(), ZIP_FL_ENC_GUESS);
        zip_delete(zip_archive, file_index);
        // Remove File Info in VFS
        VirtualBlockPtr block = FindVirtualBlock(root, dir);
        if(block == nullptr) {
            RK_WARN(File, "Virtual Block Not Existed {}, MUST Have Something Wrong", file_path);
            return false;
        }
        auto found = block->node_map.find(file_name);
        if(found == block->node_map.end()) {
            RK_WARN(File, "Virtual Node Not Existed {}, MUST Have Something Wrong", file_path);
            return false;
        } else {
            block->node_map.erase(found);
            return true;
        }
    }

    bool ZipFileSystem::CreateDir(const std::string& dir_path) {
        if(IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Create Dir in Real File System
        zip_dir_add(zip_archive, sub_path.c_str(), ZIP_FL_ENC_UTF_8);
        // Add info to VFS
        auto final_block = CreateVirtualBlock(root, dir);
        return true;
    }

    bool ZipFileSystem::RemoveDir(const std::string& dir_path) {
        if(!IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Not Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        // Remove Dir in Real File System
        auto dir_index = zip_name_locate(zip_archive, sub_path.c_str(), ZIP_FL_ENC_GUESS);
        zip_delete(zip_archive, dir_index);
        // Remove Info
        VirtualBlockPtr block = FindVirtualBlock(root, sub_path);
        if(block->parent != nullptr) {
            auto found = block->parent->block_map.find(block->name);
            block->parent->block_map.erase(found);
        } else {
            block->block_map.clear();
            block->node_map.clear();
            block->file_system.reset();
        }
        return true;
    }
}
