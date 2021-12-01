#pragma once
#include "FileSystem/OsFileSync.h"
#include "FileSystem/OsFileAsync.h"
#include "FileSystem/ZipFile.h"
#include "FileSystem/AudioFile.h"

#include <memory>

namespace Rocket {
    // TODO : Add Cache For Loaded Files
    // TODO : Finish Async File Operation
    // TODO : Upgrade This Funtion to Support Zip File Loading in Game Mode
    // TODO : Generate A Standard Pipeline:
    //        -- Parse File Name, Find Desired File Parser / No Parser
    //        -- Load File Sync/Async to Memory
    //        -- Parse File Buffer
    //        -- Add File Buffer to Global Cache, Enable GC Management
    class FileSystem {
    public:
        static void LoadSync(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer);
        //static void LoadAsync(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer);
        //static void LoadZip(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer);
        static void LoadAudio(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer);

        static void LoadSync(const std::string& path, FileOperateMode mode, FileBuffer& buffer);
        //static void LoadAsync(const std::string& path, FileOperateMode mode, FileBuffer& buffer);
        //static void LoadZip(const std::string& path, FileOperateMode mode, FileBuffer& buffer);
        static void LoadAudio(const std::string& path, FileOperateMode mode, FileBuffer& buffer);

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
