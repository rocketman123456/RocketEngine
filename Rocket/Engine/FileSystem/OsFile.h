// https://blog.molecular-matters.com/2011/08/31/file-system-part-1-platform-specific-api-design/
// https://blog.molecular-matters.com/2011/09/14/file-system-part-2-high-level-api-design/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <memory>
#include <future>

namespace Rocket {
    namespace FileSystem {
        enum class FileMode : uint32_t {
            kBinary = 0,
            kText,
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
        };

        class OsSyncFile {
        public:
            explicit OsSyncFile(OsFileHandle&& handle);
            explicit OsSyncFile(OsFileHandle&& handle, size_t length);
            OsSyncFile(const OsSyncFile& other) = delete;
            OsSyncFile& operator=(const OsSyncFile& other) = delete;
            ~OsSyncFile();
        public:
            std::unique_ptr<OsFileHandle> file_;
            std::unique_ptr<OsFileBuffer> buffer_;
        };

        class OsAsyncFile {
        public:
            explicit OsAsyncFile(OsFileHandle&& handle);
            explicit OsAsyncFile(OsFileHandle&& handle, size_t length);
            OsAsyncFile(const OsAsyncFile& other) = delete;
            OsAsyncFile& operator=(const OsAsyncFile& other) = delete;
            ~OsAsyncFile();
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
            //ReferenceCountedItem<OVERLAPPED>* m_overlapped;
        };
    }
}
