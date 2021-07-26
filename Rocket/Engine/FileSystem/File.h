#pragma once
#include <cstdlib>
#include <string>

namespace Rocket {
    enum class FileOperateMode {
        NoOperation = 0,
        ReadBinary,
        WriteBinary,
        ReadWriteBinary,
        ReadText,
        WriteText,
        ReadWriteText,
        AllOperation
    };

    struct FileBuffer {
        int64_t uuid = 0;
        int32_t size = 0;
        void*   buffer = nullptr;
    };

    struct FileHandle {
        FILE*   file;
        int64_t start;
        int64_t size;
        std::string file_name;
    };
}
