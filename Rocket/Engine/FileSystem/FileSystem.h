#pragma once
#include "FileSystem/OsFile.h"
#include "FileSystem/OsFileAsync.h"
#include "FileSystem/ZipFile.h"

#include <memory>

namespace Rocket {
    // TODO : Finish Async File Operation
    class FileSystem {
    public:
        static OsFilePtr OpenSync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static OsFileAsyncPtr OpenAsync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static ZipFilePtr OpenZip(const std::string& path, const std::string& file_name, FileOperateMode mode);

        static void CloseSync(OsFilePtr&& file);
        static void CloseAsync(OsFileAsyncPtr&& file);
        static void CloseZip(ZipFilePtr&& file);
    };
}
