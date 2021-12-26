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
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(virtual_path, &dir, &file_name, '/');
        if(file_name.length() > 0) {
            root->name = file_name;
        } else {
            root->name = "/";
        }
    }

    void ZipFileSystem::BuildVirtualSystem() {
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

    bool ZipFileSystem::CreateFile(const std::string& file_path) {
        RK_WARN(File, "Create File Not Supported");
        return false;
    }

    bool ZipFileSystem::RemoveFile(const std::string& file_path) {
        RK_WARN(File, "Remove File Not Supported");
        return false;
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
