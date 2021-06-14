#pragma once
#include "Interface/IRuntimeModule.h"
#include "FileSystem/OsFile.h"
#include "FileSystem/OsFileOperation.h"

#include <string>
#include <cstdint>
#include <future>

namespace Rocket {
    enum class ResourceType : uint32_t {
        kCompress = 0,
        kNormal,
    };

    class ResourceManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(ResourceManager);
    public:
        virtual ~ResourceManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

        std::unique_ptr<FileSystem::OsSyncFile> OpenSync(const std::string& file_name, FileSystem::FileMode mode);
        void CloseSync(std::unique_ptr<FileSystem::OsSyncFile> file);

        std::unique_ptr<FileSystem::OsAsyncFile> OpenAsync(const std::string& file_name, FileSystem::FileMode mode);
        void CloseAsync(std::unique_ptr<FileSystem::OsSyncFile> file);

    private:
        std::string root_path_;
        std::string asset_path_;
        ResourceType resource_type_;
    };

    extern IRuntimeModule* g_ResourceManager;
}