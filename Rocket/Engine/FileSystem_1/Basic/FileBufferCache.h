#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/FileBuffer.h"
#include "Container/LRUCache.h"

namespace Rocket {
    // We could make a bigger file info cache and a smaller file buffer cache
    // Use LRU to cache files
    class FileBufferCache {
    public:
        void AddResource(const std::string& name, FileBufferPtr file_handle);
        FileBufferPtr GetResource(const std::string& name);
    private:
        // Disable Copy
        FileBufferCache(const FileBufferPtr& c) = delete;
        FileBufferCache& operator = (const FileBufferPtr& c) = delete;
    private:
        LRUCache<std::string, FileBufferPtr> cache_;
    };

    void FileBufferCache::AddResource(const std::string& name, FileBufferPtr file_handle) {
        cache_.put(name, file_handle);
    }

    FileBufferPtr FileBufferCache::GetResource(const std::string& name) {
        return cache_.get(name);
    }
}
