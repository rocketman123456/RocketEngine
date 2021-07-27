#pragma once
#include "Memory/MemoryCheck.h"
#include "Core/Declare.h"

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

    Interface FileOperation {
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
        std::atomic_int32_t overlapped_;
    };
}
