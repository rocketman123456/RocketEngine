#pragma once
#include "Memory/MemoryDefine.h"

#include <cstdlib>
#include <cstdint>
#include <string>
#include <cstring>    // std::memcpy
#include <memory>

namespace Rocket {
    struct FileBuffer {
        int64_t uuid = 0;           // hash code for check
        int64_t size = 0;           // bytes
        char*   buffer = nullptr;

        FileBuffer() = default;
        ~FileBuffer() { if(buffer) delete[] buffer; } // Auto Delete

        FileBuffer(const FileBuffer& buffer) = delete;
        FileBuffer& operator = (const FileBuffer& buffer) = delete;

        // Move
        FileBuffer(FileBuffer&& buffer) {
            this->uuid = buffer.uuid;
            this->size = buffer.size;
            buffer.buffer = nullptr;
            buffer.size = 0;
        }

        // Move
        FileBuffer& operator = (FileBuffer&& buffer) {
            if (this == &buffer) return *this;
            if(this->buffer) delete [] this->buffer;
            this->uuid = buffer.uuid;
            this->size = buffer.size;
            buffer.buffer = nullptr;
            buffer.size = 0;
            return *this;
        }
    };

    using FileBufferPtr = std::shared_ptr<FileBuffer>;
}