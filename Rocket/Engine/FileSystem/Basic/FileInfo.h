#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/FileBuffer.h"
#include "Utils/StringUtils.h"
#include "Utils/Hashing.h"
#include "Log/Log.h"

//#include <gsl/gsl>

#include <cstdint>
#include <cstddef>
#include <string>
#include <memory>

namespace Rocket {
    class FileInfo {
    public:
        explicit FileInfo() = default;
        ~FileInfo() = default;

        explicit FileInfo(const std::string& file_path) : FileInfo(file_path, false) {}

        explicit FileInfo(const std::string& file_path, bool is_dir) {
            Initialize(file_path, is_dir);
        }

        explicit FileInfo(const std::string& base_path, const std::string& file_name) 
            : FileInfo(base_path, file_name, false) {}

        explicit FileInfo(const std::string& base_path, const std::string& file_name, bool is_dir) {
            Initialize(base_path, file_name, is_dir);
        }

        inline const std::string& Name() const { return name; }
        inline const std::string& BaseName() const { return base_name; }
        inline const std::string& BasePath() const { return base_path; }
        inline const std::string& Extension() const { return extension; }
        inline const std::string& AbsolutePath() const { return absolute_path; }
        inline bool IsDir() const { return is_dir; }
        inline bool IsValid() const { return is_valid; }
        inline int64_t Hash() const { return hash_code; }
        inline int64_t HashExt() const { return hash_ext; }

    private:
        void Initialize(const std::string& file_path, bool is_dir) noexcept {
            std::size_t found = file_path.rfind("/");
            if (found != std::string::npos) {
                const std::string base_path = file_path.substr(0, found + 1);
                std::string file_name = "";
                if (found != file_path.length()) {
                    file_name = file_path.substr(found + 1, file_path.length() - found - 1);
                }
                Initialize(base_path, file_name, is_dir);
            }
        }

        void Initialize(const std::string& base_path_, const std::string& file_name_, bool is_dir_) noexcept {
            this->base_path = base_path_;
            this->name = file_name_;
            this->is_dir = is_dir_;

            // Standardize names
            if (!EndsWith(this->base_path, "/")) { this->base_path += "/"; }
            if (this->is_dir && !EndsWith(this->name, "/")) { this->name += "/"; }
            if (StartsWith(this->name, "/")) { this->name = this->name.substr(1, this->name.length() - 1); }

            absolute_path = base_path + name;

            // find file name and type
            std::size_t dots_num = std::count(name.begin(), name.end(), '.');
            bool is_real_dir = (dots_num == name.length() && is_dir);
            if(!is_real_dir) {
                std::size_t found = name.rfind(".");
                if(found != std::string::npos) {
                    base_name = name.substr(0, found);
                    if(found < name.length()) {
                        extension = name.substr(found, name.length() - found);
                        hash_ext = hash_i64(extension);
                    }
                }
            }

            // Finally, hash code
            hash_code = hash_i64(absolute_path);
        }

    private:
        std::string name = "";          // File Name
        std::string base_path = "";     // File Basic Path
        std::string base_name = "";     // File Name Without type
        std::string extension = "";     // File Type
        std::string absolute_path = ""; // File Full Path
        int64_t     hash_code = -1;     // File Hash Code
        int64_t     hash_ext = -1;
        bool        is_dir = false;
        bool        is_valid = true;
    };

    inline bool operator == (const FileInfo& fi1, const FileInfo& fi2) { return fi1.Hash() == fi2.Hash(); }
    inline bool operator < (const FileInfo& fi1, const FileInfo& fi2) { return fi1.Hash() < fi2.Hash(); }
    
    CLASS_PTR(FileInfo);
}
