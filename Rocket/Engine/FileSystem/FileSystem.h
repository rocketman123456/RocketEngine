#pragma once
#include "FileSystem/OsFileSync.h"
#include "FileSystem/OsFileAsync.h"
#include "FileSystem/ZipFile.h"
#include "FileSystem/AudioFile.h"

#include <memory>

namespace Rocket {
    // TODO : Finish Async File Operation
    class FileSystem {
    public:
        static OsFileSyncPtr OpenSync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static OsFileAsyncPtr OpenAsync(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static ZipFilePtr OpenZip(const std::string& path, const std::string& file_name, FileOperateMode mode);
        static AudioFilePtr OpenAudio(const std::string& path, const std::string& file_name, FileOperateMode mode);

        static OsFileSyncPtr OpenSync(const std::string& path, FileOperateMode mode);
        static OsFileAsyncPtr OpenAsync(const std::string& path, FileOperateMode mode);
        static ZipFilePtr OpenZip(const std::string& path, FileOperateMode mode);
        static AudioFilePtr OpenAudio(const std::string& path, FileOperateMode mode);

        static void CloseSync(OsFileSyncPtr&& file);
        static void CloseAsync(OsFileAsyncPtr&& file);
        static void CloseZip(ZipFilePtr&& file);
        static void CloseAudio(AudioFilePtr&& file);
    };
}
