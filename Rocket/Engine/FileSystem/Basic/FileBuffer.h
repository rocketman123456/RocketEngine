#pragma once
#include "Memory/MemoryDefine.h"

#include <gsl/gsl>

#include <cstdlib>
#include <cstdint>
#include <memory>

namespace Rocket {
    //Should Manage Memory by User
    struct FileBuffer {
        // hash code for quicker find
        int64_t uuid = 0;
        gsl::span<gsl::byte> buffer;

        ~FileBuffer() {
            if(buffer.data()) {
                delete [] buffer.data();
            }
        }

        // Uncopyable
        FileBuffer(const FileBuffer& buffer) = delete;
        FileBuffer& operator = (const FileBuffer& buffer) = delete;
        // Can Only Move
        FileBuffer(FileBuffer&& buffer) = default;
        FileBuffer& operator = (FileBuffer&& buffer) = default;
    };

    using FileBufferPtr = std::shared_ptr<FileBuffer>;
}
