#pragma once

#include <vector>
#include <cstdint>

#include <ospray/ospray_cpp.h>
#include <ospray/ospray_cpp/ext/rkcommon.h>

class OsprayBackend {
public:
    OsprayBackend() = default;

    // Call once after ospInit() in main()
    void init();

    // Resize framebuffer + update camera aspect
    void resize(int w, int h);

    // Set camera look-at
    void setCamera(const rkcommon::math::vec3f& eye,
        const rkcommon::math::vec3f& center,
        const rkcommon::math::vec3f& up,
        float fovyDeg);

    // Clear accumulation (call when camera changes)
    void resetAccumulation();

    // Render one frame and return pointer to RGBA8 pixels
    // Valid until next render/resize.
    const uint32_t* render();

    int width() const { return fbW_; }
    int height() const { return fbH_; }

private:
    void buildTestMeshScene();

    int fbW_ = 1;
    int fbH_ = 1;

    // OSPRay objects
    ospray::cpp::Renderer renderer_;
    ospray::cpp::Camera camera_;
    ospray::cpp::World world_;
    ospray::cpp::FrameBuffer fb_;

    // CPU-side pixel buffer (RGBA8)
    std::vector<uint32_t> pixels_;
};