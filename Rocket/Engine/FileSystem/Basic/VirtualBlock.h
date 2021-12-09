#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/FileInfo.h"

#include <unordered_map>

namespace Rocket {
    // Store File System Info, Just like linux block and inode
    class VirtualBlock {
    public:
        using SubDirectoryMap = std::unordered_map<std::string, Directory>;
        using FileInfoMap = std::unordered_map<std::string, Directory>;
    public:

    private:
        SubDirectoryMap dir_map;
    };
}
