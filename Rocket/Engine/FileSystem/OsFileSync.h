#pragma once
#include "FileSystem/OsFile.h"

#include <cstdint>
#include <string>
#include <cstdlib>
#include <atomic>
#include <memory>

namespace Rocket {
    class OsFileSync : _implements_ OsFile {
    public:
        OsFileSync() = default;
        virtual ~OsFileSync() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode);
        virtual void Finalize();
        ////////////////////////////////////////////////////////////////////////
        // Synchronous API
        ////////////////////////////////////////////////////////////////////////

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        virtual std::size_t Read(FileBuffer& buffer, std::size_t length);
        virtual std::size_t ReadAll(FileBuffer& buffer);
        /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
        virtual std::size_t Write(FileBuffer& buffer, std::size_t length);
        virtual std::size_t WriteAll(FileBuffer& buffer);
        /// Seeks to the desired position
        virtual void Seek(std::size_t position);
        /// Seeks to the end of the file
        virtual void SeekToEnd(void);
        /// Skips a certain amount of bytes
        virtual void Skip(std::size_t bytes);
        /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
        virtual std::size_t Tell(void) const;
    };

    using OsFilePtr = std::unique_ptr<OsFileSync>;
}
