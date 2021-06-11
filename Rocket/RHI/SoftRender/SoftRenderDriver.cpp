#include "SoftRender/SoftRenderDriver.h"

namespace Rocket {
    void SoftRenderDriver::LoadFragmentShader(std::function<void()> shader) {
        frag_shader_ = shader;
    }

    void SoftRenderDriver::LoadVertexShader(std::function<void()> shader) {
        vert_shader_ = shader;
    }

    void SoftRenderDriver::LoadGeometryShader(std::function<void()> shader) {
        geom_shader_ = shader;
    }

    void SoftRenderDriver::LoadComputeShader(std::function<void()> shader) {
        comp_shader_ = shader;
    }

    void SoftRenderDriver::InputTriangles() {

    }

    void SoftRenderDriver::Rasterize() {

    }

    void SoftRenderDriver::Draw() {

    }
}