#include "FileSystem/ZipFile.h"

namespace Rocket {
    ZipAsyncFileOperation::ZipAsyncFileOperation(const ZipAsyncFileOperation& other) : file_(other.file_) {
        int32_t temp = other.overlapped_;
        overlapped_ = temp;
        overlapped_++; 
    }

    ZipAsyncFileOperation& ZipAsyncFileOperation::operator=(const ZipAsyncFileOperation& other) {
        if (this != &other) {
            file_ = other.file_;
            int32_t temp = other.overlapped_;
            overlapped_ = temp;
            overlapped_++;
        }
        return *this;
    }
}
