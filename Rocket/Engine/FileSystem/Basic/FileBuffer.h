#pragma once
#include "Core/MemoryDefine.h"

#include <gsl/gsl>

#include <cstdlib>
#include <cstdint>
#include <memory>

namespace Rocket {
    //Should Manage Memory by User
    struct FileBuffer {
        int64_t uuid = 0;               // Global Resource Id, to Identify a File, Using Hash of full path
        gsl::span<gsl::byte> buffer;    // Store file content, can't be changed during runtime

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

    CLASS_PTR(FileBuffer);
}
