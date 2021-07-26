#pragma once
#include "FileSystem/File.h"

#include <cstdint>
#include <string>
#include <cstdlib>
#include <atomic>

namespace Rocket {
    class OsAsyncFileOperation {
    public:
        OsAsyncFileOperation(const FileHandle& file, size_t position);
        OsAsyncFileOperation(const OsAsyncFileOperation& other);
        OsAsyncFileOperation& operator=(const OsAsyncFileOperation& other);
        ~OsAsyncFileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        bool HasFinished() const;
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        size_t WaitUntilFinished() const;
        /// Cancels the asynchronous operation
        void Cancel();
    private:
        FileHandle file_;
        std::atomic_int32_t overlapped_;
    };

    class OsFile {
    public:
        OsFile(const std::string& path, FileOperateMode mode);
        ~OsFile();
        ////////////////////////////////////////////////////////////////////////
        // Synchronous API
        ////////////////////////////////////////////////////////////////////////

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        std::size_t Read(FileBuffer& buffer, std::size_t length);
        /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
        std::size_t Write(FileBuffer& buffer, std::size_t length);
        /// Seeks to the desired position
        void Seek(std::size_t position);
        /// Seeks to the end of the file
        void SeekToEnd(void);
        /// Skips a certain amount of bytes
        void Skip(std::size_t bytes);
        /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
        std::size_t Tell(void) const;
    protected:
        std::string file_name_;
        FileOperateMode mode_;
        FileHandle file_;
    };

    class OsFileAsync : public OsFile {
    public:
        OsFileAsync(const std::string& path, FileOperateMode mode, bool async);
        ~OsFileAsync();
        ////////////////////////////////////////////////////////////////////////
        // Asynchronous API
        ////////////////////////////////////////////////////////////////////////

        /// Asynchronously reads from the file into a buffer
        OsAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        OsAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);
    protected:
        bool async_;
    };
}
