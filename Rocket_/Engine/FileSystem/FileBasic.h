#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <cstdlib>
#include <atomic>
#include <cstdint>
#include <string>
#include <string_view>
#include <cstring>    // std::memcpy
#include <memory>

namespace Rocket {
    enum class FileOperateMode : int8_t {
        EMPTY_OPERATION = 0,
        READ_BINARY,
        WRITE_BINARY,
        READWRITE_BINARY,
        READ_TEXT,
        WRITE_TEXT,
        READWRITE_TEXT,
        OPERATION_COUNT,
    };

    struct FileBuffer {
        int64_t uuid = 0;           // hash code for check
        int64_t size = 0;           // bytes
        void*   buffer = nullptr;

        FileBuffer() = default;
        ~FileBuffer() { if(buffer) delete[] buffer; } // Auto Delete

        FileBuffer(const FileBuffer& buffer) = delete;
        FileBuffer& operator = (const FileBuffer& buffer) = delete;

        // Always Deep Copy
        // FileBuffer(const FileBuffer& buffer) {
        //     this->buffer = new int8_t[buffer.size];
        //     this->size = buffer.size;
        //     std::memcpy(this->buffer, buffer.buffer, size);
        // }
        // Move
        FileBuffer(FileBuffer&& buffer) {
            this->uuid = buffer.uuid;
            this->size = buffer.size;
            buffer.buffer = nullptr;
            buffer.size = 0;
        }

        // Copy
        // FileBuffer& operator = (const FileBuffer& buffer) {
        //     if (this == &buffer)
        //         return *this;
        //     if(this->buffer)
        //         delete [] this->buffer;
        //     this->buffer = new int8_t[buffer.size];
        //     this->size = buffer.size;
        //     std::memcpy(this->buffer, buffer.buffer, size);
        //     return *this;
        // }
        // Move
        FileBuffer& operator = (FileBuffer&& buffer) {
            if (this == &buffer)
                return *this;
            if(this->buffer)
                delete [] this->buffer;
            this->uuid = buffer.uuid;
            this->size = buffer.size;
            buffer.buffer = nullptr;
            buffer.size = 0;
            return *this;
        }
    };

    using FileBufferPtr = std::shared_ptr<FileBuffer>;

    struct FileHandle {
        void* file_pointer = nullptr;
        void* extra_file_info = nullptr;
        int64_t total_size;
        std::string file_path;
        std::string file_name;
        std::string full_name;
    };

    _Interface_ FileOperation {
    public:
        virtual ~FileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        virtual bool HasFinished() const = 0;
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        virtual size_t WaitUntilFinished() const = 0;
        /// Cancels the asynchronous operation
        virtual void Cancel() = 0;
    protected:
        FileHandle file_;
        std::atomic<int32_t> overlapped_;
    };
}
