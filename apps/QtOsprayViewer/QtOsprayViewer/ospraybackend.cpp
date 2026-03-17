#include "ospraybackend.h"

#include <algorithm>
#include <cstring>

using rkcommon::math::vec3f;
using rkcommon::math::vec3ui;
using rkcommon::math::vec4f;

void OsprayBackend::init()
{
  // Renderer
  renderer_ = ospray::cpp::Renderer("scivis");
  renderer_.setParam("aoSamples", 0); // clean image
  renderer_.setParam("backgroundColor", vec3f(1.0f)); // white
  renderer_.commit();

  // Camera
  camera_ = ospray::cpp::Camera("perspective");
  camera_.setParam("fovy", 60.f);
  camera_.commit();

  // World/scene
  buildTestMeshScene();
}

void OsprayBackend::buildTestMeshScene()
{
  // Mesh from ospTutorial
  std::vector<vec3f> vertex = {vec3f(-1.0f, -1.0f, 3.0f),
      vec3f(-1.0f, 1.0f, 3.0f),
      vec3f(1.0f, -1.0f, 3.0f),
      vec3f(0.1f, 0.1f, 0.3f)};

  std::vector<vec4f> color = {vec4f(0.9f, 0.5f, 0.5f, 1.0f),
      vec4f(0.8f, 0.8f, 0.8f, 1.0f),
      vec4f(0.8f, 0.8f, 0.8f, 1.0f),
      vec4f(0.5f, 0.9f, 0.5f, 1.0f)};

  std::vector<vec3ui> index = {vec3ui(0, 1, 2), vec3ui(1, 2, 3)};

  ospray::cpp::Geometry mesh("mesh");
  mesh.setParam("vertex.position", ospray::cpp::CopiedData(vertex));
  mesh.setParam("vertex.color", ospray::cpp::CopiedData(color));
  mesh.setParam("index", ospray::cpp::CopiedData(index));
  mesh.commit();

  ospray::cpp::GeometricModel model(mesh);
  model.commit();

  std::vector<ospray::cpp::GeometricModel> models = {model};
  ospray::cpp::Group group;
  group.setParam("geometry", ospray::cpp::CopiedData(models));
  group.commit();

  ospray::cpp::Instance instance(group);
  instance.commit();

  std::vector<ospray::cpp::Instance> instances = {instance};
  world_ = ospray::cpp::World();
  world_.setParam("instance", ospray::cpp::CopiedData(instances));

  // Ambient light
  ospray::cpp::Light light("ambient");
  light.commit();
  std::vector<ospray::cpp::Light> lights = {light};
  world_.setParam("light", ospray::cpp::CopiedData(lights));

  world_.commit();
}

void OsprayBackend::resize(int w, int h)
{
  fbW_ = std::max(1, w);
  fbH_ = std::max(1, h);

  camera_.setParam("aspect", float(fbW_) / float(fbH_));
  camera_.commit();

  fb_ = ospray::cpp::FrameBuffer(
      fbW_, fbH_, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);
  fb_.clear();

  pixels_.assign(size_t(fbW_) * size_t(fbH_), 0u);
}

void OsprayBackend::setCamera(
    const vec3f &eye, const vec3f &center, const vec3f &up, float fovyDeg)
{
  camera_.setParam("position", eye);
  camera_.setParam("direction", center - eye);
  camera_.setParam("up", up);
  camera_.setParam("fovy", fovyDeg);
  camera_.commit();
}

void OsprayBackend::resetAccumulation()
{
  if (fb_.handle())
    fb_.clear();
}

const uint32_t *OsprayBackend::render()
{
  fb_.renderFrame(renderer_, camera_, world_);

  void *mapped = fb_.map(OSP_FB_COLOR);
  std::memcpy(pixels_.data(), mapped, pixels_.size() * sizeof(uint32_t));
  fb_.unmap(mapped);

  return pixels_.data();
}