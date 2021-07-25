#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace Rocket {
    class ZipFile {
    public:
        ZipFile();
        virtual ~ZipFile();

        [[nodiscard]] int32_t Initialize(const std::string& file_name);
        void Finalize();

        int32_t GetNumFiles() const;
        std::string GetFilename(int32_t i) const;	
        int32_t GetFileLen(int32_t i) const;
        bool ReadFile(int32_t i, void* buffer);

        // Added to show multi-threaded decompression
        bool ReadLargeFile(int32_t i, void* buffer, std::function<void(int32_t, bool&)> call_back);

        int32_t Find(const std::string& path) const;

    private:
        std::string file_name_;
    };
}
