#pragma once

#include <stb_image.h>
#include <stb_image_resize.h>
#include <stb_image_write.h>

#include <string>
#include <string_view>

namespace Rocket {
    class ImageParser {
    public:
        ImageParser() = default;
        ImageParser(const std::string& full) : full_path_(full) {}
        ImageParser(const std::string& path, const std::string& name) : path_(path), name_(name), full_path_(path + name) {}
        ~ImageParser() = default;

        void Parse();

    private:
        std::string_view path_;
        std::string_view name_;
        std::string_view full_path_;
    };
}
