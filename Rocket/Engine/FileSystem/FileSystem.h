#pragma once
#include "FileSystem/OsFile.h"

#include <memory>
#include <future>

namespace Rocket {
    class FileSystem {
    public:
        std::unique_ptr<OsFile> OpenSync();
        std::unique_ptr<OsFileAsync> OpenAsync();

        void CloseSync(std::unique_ptr<OsFile>&& file);
        void CloseAsync(std::unique_ptr<OsFileAsync>&& file);
    private:
        // TODO : register parser for different file
    };
}
