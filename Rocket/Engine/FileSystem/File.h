#pragma once
#include "Memory/MemoryCheck.h"

#include <cstdlib>
#include <string>

namespace Rocket {
    enum class FileOperateMode : int8_t {
        NoOperation = 0,
        ReadBinary,
        WriteBinary,
        //ReadWriteBinary,
        ReadText,
        WriteText,
        //ReadWriteText,
        //AllOperation
    };

    struct FileBuffer {
        int64_t uuid = 0;
        int64_t size = 0;
        void*   buffer = nullptr;
    };

    struct FileHandle {
        void*   file_pointer = nullptr;
        int64_t total_size;
        std::string file_path;
        std::string file_name;
        std::string full_name;
    };
}
