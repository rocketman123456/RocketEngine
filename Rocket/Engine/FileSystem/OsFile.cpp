#include "FileSystem/OsFile.h"

namespace Rocket {
    OsAsyncFileOperation::OsAsyncFileOperation(const OsAsyncFileOperation& other) : file_(other.file_) {
        int32_t temp = other.overlapped_;
        overlapped_ = temp;
        overlapped_++; 
    }

    OsAsyncFileOperation& OsAsyncFileOperation::operator=(const OsAsyncFileOperation& other) {
        if (this != &other) {
            file_ = other.file_;
            int32_t temp = other.overlapped_;
            overlapped_ = temp;
            overlapped_++;
        }
        return *this;
    }
}
