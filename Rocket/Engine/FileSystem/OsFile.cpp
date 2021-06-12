#include "FileSystem/OsFile.h"

namespace Rocket {
    namespace FileSystem {
        OsFileAsyncOperation::OsFileAsyncOperation(const OsFileAsyncOperation& other)
            : file_(other.file_) {//, m_overlapped(other.m_overlapped) {
            //m_overlapped->AddReference();
        }
        
        OsFileAsyncOperation& OsFileAsyncOperation::operator=(const OsFileAsyncOperation& other) {
            if (this != &other) {
                file_ = other.file_;
                //m_overlapped = other.m_overlapped;
                //m_overlapped->AddReference();
            }
            
            return *this;
        }
        
        OsFileAsyncOperation::~OsFileAsyncOperation(void) {
            //if (m_overlapped->RemoveReference() == 0)
            //    delete m_overlapped;
        }
    }
}
