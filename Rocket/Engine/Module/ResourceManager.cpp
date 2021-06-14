#include "Module/ResourceManager.h"
#include "Parser/ConfigParser.h"

namespace Rocket {
    extern ConfigParser* g_ConfigParser;

    int32_t ResourceManager::Initialize() {
        root_path_ = g_ConfigParser->GetRootPath();
        asset_path_ = root_path_ + "/Asset/";
        std::string type = g_ConfigParser->GetConfigInfo<std::string>("basic", "resource-type");
        RK_CORE_INFO("Resource Type : {}", type);
        return 0;
    }

    void ResourceManager::Finalize() {
        
    }

    void ResourceManager::Tick(TimeStep ts) {
        // DO nothing
    }

    std::unique_ptr<FileSystem::OsSyncFile> ResourceManager::OpenSync(const std::string& file_name, FileSystem::FileMode mode) {
        FILE* file = nullptr;
        std::string file_open_mode;
        if(mode == (FileSystem::FileMode::kBinary | FileSystem::FileMode::kRead)) {
            file_open_mode = "rb";
        }
        else if(mode == (FileSystem::FileMode::kText | FileSystem::FileMode::kRead)) {
            file_open_mode = "r";
        }
        else if(mode == (FileSystem::FileMode::kBinary | FileSystem::FileMode::kWrite)) {
            file_open_mode = "wb";
        }
        else if(mode == (FileSystem::FileMode::kText | FileSystem::FileMode::kWrite)) {
            file_open_mode = "w";
        }
        else if(mode == (FileSystem::FileMode::kBinary | FileSystem::FileMode::kRead | FileSystem::FileMode::kWrite)) {
            file_open_mode = "rwb";
        }
        else if(mode == (FileSystem::FileMode::kText | FileSystem::FileMode::kRead | FileSystem::FileMode::kWrite)) {
            file_open_mode = "rw";
        }
        file = fopen(file_name.c_str(), file_open_mode.c_str());
        std::unique_ptr<FileSystem::OsFileHandle> file_handle(new FileSystem::OsFileHandle);
        file_handle->file = file;
        file_handle->file_path = file_name;
        file_handle->mode = mode;
        std::unique_ptr<FileSystem::OsSyncFile> target_file(new FileSystem::OsSyncFile(std::move(file_handle)));
        return std::move(target_file);
    }

    void ResourceManager::CloseSync(std::unique_ptr<FileSystem::OsSyncFile> file) {
        if(file->file_->file != nullptr) {
            fclose(file->file_->file);
        }
    }

    std::unique_ptr<FileSystem::OsAsyncFile> ResourceManager::OpenAsync(const std::string& file_name, FileSystem::FileMode mode) {
        return nullptr;
    }

    void ResourceManager::CloseAsync(std::unique_ptr<FileSystem::OsSyncFile> file) {
    }
}