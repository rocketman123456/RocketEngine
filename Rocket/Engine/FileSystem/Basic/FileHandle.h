#pragma once
#include "Memory/MemoryDefine.h"
#include "FileSystem/Basic/FileBuffer.h"

#include <gsl/span>
#include <gsl/owner>

#include <cstdint>
#include <cstddef>
#include <string>
#include <memory>

namespace Rocket {
    struct FileHandle {
        gsl::span<char> file_info;
        gsl::span<char> extra_file_info;
        std::string file_path = "";  // Directory
        std::string file_name = "";  // File Name
        std::string full_name = "";  // Complete Name
        gsl::owner<FileBuffer> buffer;
    };
}
