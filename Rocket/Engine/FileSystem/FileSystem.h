#pragma once
#include "FileSystem/OsFile.h"
#include "FileSystem/OsFileAsync.h"
#include "FileSystem/ZipFile.h"

#include <memory>

namespace Rocket {
    class FileSystem {
    public:
        static std::unique_ptr<OsFile> OpenSync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static std::unique_ptr<OsFileAsync> OpenAsync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static std::unique_ptr<ZipFile> OpenZip(const std::string& path, const std::string& file_name, FileOperateMode mode);

        static void CloseSync(std::unique_ptr<OsFile>&& file);
        static void CloseAsync(std::unique_ptr<OsFileAsync>&& file);
        static void CloseZip(std::unique_ptr<ZipFile>&& file);
    };
}
