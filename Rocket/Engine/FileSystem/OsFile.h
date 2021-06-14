// https://blog.molecular-matters.com/2011/08/31/file-system-part-1-platform-specific-api-design/
// https://blog.molecular-matters.com/2011/09/14/file-system-part-2-high-level-api-design/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <memory>
#include <future>
#include <atomic>

namespace Rocket {
    namespace FileSystem {
        enum FileMode : uint32_t {
            kBinary = 1 << 0,
            kText = 1 << 1,
            kRead = 1 << 2,
            kWrite = 1 << 3,
        };

        enum class FilePos : uint32_t {
            kFileBegin = 0,
            kFileCurrent,
            kFileEnd,
        };

        struct OsFileBuffer {
            void* data;
            size_t length;
        };

        struct OsFileHandle {
            FILE* file;
            FileMode mode;
            std::string file_path;
        };

        class OsSyncFile {
        public:
            explicit OsSyncFile(std::unique_ptr<OsFileHandle> handle);
            explicit OsSyncFile(std::unique_ptr<OsFileHandle> handle, size_t length);
            OsSyncFile(OsSyncFile&& other);
            OsSyncFile(OsSyncFile& other);
            ~OsSyncFile();

            /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
            size_t ReadSync(OsFileBuffer* buffer, size_t length);
            /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
            size_t WriteSync(const OsFileBuffer* buffer, size_t length);
            /// Seeks to the desired position
            void Seek(size_t position);
            /// Seeks to the end of the file
            void SeekToEnd(void);
            /// Skips a certain amount of bytes
            void Skip(size_t bytes);
            /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
            size_t Tell(void) const;

            inline OsFileHandle* GetFileHandle() { return file_.get(); }
            inline OsFileBuffer* GetFileBuffer() { return buffer_.get(); }

            OsSyncFile& operator=(const OsSyncFile& other) = delete;
        public:
            std::unique_ptr<OsFileHandle> file_;
            std::unique_ptr<OsFileBuffer> buffer_;
        };

        class OsFileAsyncOperation;

        class OsAsyncFile {
        public:
            explicit OsAsyncFile(std::unique_ptr<OsFileHandle> handle);
            explicit OsAsyncFile(std::unique_ptr<OsFileHandle> handle, size_t length);
            OsAsyncFile(OsAsyncFile&& other);
            OsAsyncFile(OsAsyncFile& other);
            ~OsAsyncFile();

            /// Asynchronously reads from the file into a buffer
            OsFileAsyncOperation ReadAsync(OsFileBuffer* buffer, size_t length);
            /// Asynchronously writes from a buffer into the file
            OsFileAsyncOperation WriteAsync(const OsFileBuffer* buffer, size_t length);
            /// Waits until the asynchronous operation has finished
            void WaitForAsync(OsFileAsyncOperation operation);
            /// Returns the number of bytes transferred in the asynchronous operation
            size_t GetAsyncBytesTransferred(OsFileAsyncOperation operation) const;

            inline OsFileHandle* GetFileHandle() { return file_.get(); }
            inline OsFileBuffer* GetFileBuffer() { return buffer_.get(); }

            OsAsyncFile& operator=(const OsAsyncFile& other) = delete;
        public:
            std::unique_ptr<OsFileHandle> file_;
            std::unique_ptr<OsFileBuffer> buffer_;
        };

        class OsFileAsyncOperation {
        public:
            OsFileAsyncOperation(OsAsyncFile* file, size_t position);
            OsFileAsyncOperation(const OsFileAsyncOperation& other);
            OsFileAsyncOperation& operator=(const OsFileAsyncOperation& other);
            ~OsFileAsyncOperation();
            
            /// Returns whether or not the asynchronous operation has finished
            bool HasFinished(void) const;
            /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
            size_t WaitUntilFinished(void) const;
            /// Cancels the asynchronous operation
            void Cancel(void);
        
        private:
            OsAsyncFile* file_;
            std::atomic_int32_t ref_count_;
        };
    }
}
