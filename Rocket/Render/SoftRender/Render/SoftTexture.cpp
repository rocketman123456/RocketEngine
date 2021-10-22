#include "Render/SoftTexture.h"
#include "Parser/ImageParser.h"

namespace Rocket {
    SoftTexture::SoftTexture(const std::string& path, const std::string& name) {
        ImageParser parser(path, name);
        parser.Parse(image_data);
    }

    SoftTexture::SoftTexture(const std::string& full_path) {
        ImageParser parser(full_path);
        parser.Parse(image_data);
    }

    Eigen::Vector4f SoftTexture::GetColorRGBA(float u, float v) {
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
        int32_t u_img = u * width;
        int32_t v_img = (1 - v) * height;
        return GetPixelRGBA(u_img, v_img);
    }

    Eigen::Vector3f SoftTexture::GetColorRGB(float u, float v) {
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
        int32_t u_img = u * width;
        int32_t v_img = (1 - v) * height;
        return GetPixelRGB(u_img, v_img);
    }

    Eigen::Vector4f SoftTexture::GetPixelRGBA(int32_t x, int32_t y) {
        auto color = image_data[y * width + x];
        return color * 255.f;
    }

    Eigen::Vector3f SoftTexture::GetPixelRGB(int32_t x, int32_t y) {
        auto color = image_data[y * width + x];
        return color.head<3>() * 255.f;
    }
}