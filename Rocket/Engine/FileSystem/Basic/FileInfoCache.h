#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/FileInfo.h"
#include "Container/LRUCache.h"

namespace Rocket {
    // We could make a bigger file info cache and a smaller file buffer cache
    // Use LRU to cache files
    class FileInfoCache {
    public:
        void AddResource(const std::string& name, FileInfoPtr file_handle);
        // If FileInfoPtr is null, Not in Cache now
        FileInfoPtr GetResource(const std::string& name);
    private:
        // Disable Copy
        FileInfoCache(const FileInfoCache& c) = delete;
        FileInfoCache& operator = (const FileInfoCache& c) = delete;
    private:
        LRUCache<std::string, FileInfoPtr> cache_;
    };

    void FileInfoCache::AddResource(const std::string& name, FileInfoPtr file_handle) {
        cache_.put(name, file_handle);
    }

    FileInfoPtr FileInfoCache::GetResource(const std::string& name) {
        return cache_.get(name);
    }
}
