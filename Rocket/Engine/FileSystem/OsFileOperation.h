#pragma once

#include "FileSystem/OsFile.h"

namespace Rocket {
    namespace FileSystem {
        class OsFileOperation {
        public:
            OsFileOperation(const char* path, FileMode mode, bool async);
            ~OsFileOperation( );
            
            ////////////////////////////////////////////////////////////////////////
            // Synchronous API
            ////////////////////////////////////////////////////////////////////////
            
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
            
            ////////////////////////////////////////////////////////////////////////
            // Asynchronous API
            ////////////////////////////////////////////////////////////////////////
            
            /// Asynchronously reads from the file into a buffer
            OsFileAsyncOperation ReadAsync(OsFileBuffer* buffer, size_t length);
            
            /// Asynchronously writes from a buffer into the file
            OsFileAsyncOperation WriteAsync(const OsFileBuffer* buffer, size_t length);
            
            /// Waits until the asynchronous operation has finished
            void WaitForAsync(OsFileAsyncOperation operation);
            
            /// Returns the number of bytes transferred in the asynchronous operation
            size_t GetAsyncBytesTransferred(OsFileAsyncOperation operation) const;

            private:

        };
    }
}
