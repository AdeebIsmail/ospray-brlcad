// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //
// RTC_GEOMETRY_TYPE_USER
#include "brlcad.h"
#include "brlcad_ispc.h"

#include "ospray/common/Data.h"
#include "ospray/common/Model.h"
#include "ospray/common/Ray.h"

#include "ospcommon/tasking/tasking_system_handle.h"
#include "ospcommon/utility/StringManip.h"

#include <atomic>
#include <thread>
#include <unordered_map>

namespace ospray {
namespace brl_cad {

static inline int getCpuId() {
  return std::hash<std::thread::id>()(std::this_thread::get_id()) % MAX_PSW;
#if 0
      static std::unordered_map<std::thread::id, int> threadIds;
      static std::atomic<int> nextId{1};
      static std::mutex mtx;


      mtx.lock();
      const auto currentThread = std::this_thread::get_id();
      auto id = threadIds[currentThread];
      if (id == 0) {
	id = ++nextId;
	threadIds[currentThread] = id;
      }
      mtx.unlock();

      return id - 1;
#endif
}

// Local helper functions /////////////////////////////////////////////////

template <typename T>
inline static void getRay(const T &rays, RTCRayHit &rayhit, int i) {
  // Update to use RTCRayHit, which includes the RTCRay
  auto &ray = rayhit.ray;
  auto &hit = rayhit.hit;

  ray.org_x = rays.orgx[i];
  ray.org_y = rays.orgy[i];
  ray.org_z = rays.orgz[i];

  ray.dir_x = rays.dirx[i];
  ray.dir_y = rays.diry[i];
  ray.dir_z = rays.dirz[i];

  ray.tnear = rays.tnear[i];
  ray.tfar = rays.tfar[i];

  ray.time = rays.time[i];
  ray.mask = rays.mask[i];
  ray.flags = 0;

  hit.primID = rays.primID[i];
  hit.geomID = rays.geomID[i];
  hit.instID[0] = rays.instID[i];
}

template <typename T>
inline static void setRay(const RTCRayHit &rayhit, T &rays, int i) {
  // Update to use RTCRayHit, which includes the RTCRay

  const auto &ray = rayhit.ray;
  const auto &hit = rayhit.hit;

  if (hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    rays.Ngx[i] = hit.Ng_x;
    rays.Ngy[i] = hit.Ng_y;
    rays.Ngz[i] = hit.Ng_z;

    rays.primID[i] = hit.primID;
    rays.geomID[i] = hit.geomID;
    rays.instID[i] = hit.instID[0];
    rays.tfar[i] = ray.tfar;
    rays.u[i] = hit.u;
    rays.v[i] = hit.v;
  }
}

static int hitCallback(application *ap, partition *PartHeadp, seg *segs) {
  /* will contain surface curvature information at the entry */
#if 0
      curvature cur = RT_CURVATURE_INIT_ZERO;
#endif

  auto &rayhit = *static_cast<RTCRayHit *>(ap->a_uptr);
  auto &ray = rayhit.ray;
  auto &hit = rayhit.hit;

  /* iterate over each partition until we get back to the head.
   * each partition corresponds to a specific homogeneous region of
   * material.
   */
  for (auto *pp = PartHeadp->pt_forw; pp != PartHeadp; pp = pp->pt_forw) {
    /* entry hit point, so we type less */
    auto *hitp = pp->pt_inhit;

#if 0
        /* construct the actual (entry) hit-point from the ray and the
         * distance to the intersection point (i.e., the 't' value).
         */
        point_t pt;
        VJOIN1(pt, ap->a_ray.r_pt, hitp->hit_dist, ap->a_ray.r_dir);
#endif

    /* primitive we encountered on entry */
    auto *stp = pp->pt_inseg->seg_stp;

    /* compute the normal vector at the entry point, flipping the
     * normal if necessary.
     */
    vect_t inormal;
    RT_HIT_NORMAL(inormal, hitp, stp, &(ap->a_ray), pp->pt_inflip);

    ray.tfar = hitp->hit_dist;
    hit.Ng_x = inormal[0];
    hit.Ng_y = inormal[1];
    hit.Ng_z = inormal[2];
    return 1;

    /* ...COLOR... */
    // pp->pt_regionp->reg_mater->ma_color
#if 0
        /* This next macro fills in the curvature information which
         * consists on a principle direction vector, and the inverse
         * radii of curvature along that direction and perpendicular
         * to it.  Positive curvature bends toward the outward
         * pointing normal.
         */
        RT_CURVATURE(&cur, hitp, pp->pt_inflip, stp);

        /* exit point, so we type less */
        hitp = pp->pt_outhit;

        /* construct the actual (exit) hit-point from the ray and the
         * distance to the intersection point (i.e., the 't' value).
         */
        VJOIN1(pt, ap->a_ray.r_pt, hitp->hit_dist, ap->a_ray.r_dir);

        /* primitive we exited from */
        stp = pp->pt_outseg->seg_stp;

        /* compute the normal vector at the exit point, flipping the
         * normal if necessary.
         */
        vect_t onormal;
        RT_HIT_NORMAL(onormal, hitp, stp, &(ap->a_ray), pp->pt_outflip);
#endif
  }

  // Return '1' for hit
  return 1;
}

static int missCallback(application *ap) {
  // Return '0' for miss
  return 0;
}

static void traceRay(const BRLCAD &geom, RTCRayHit &rayhit) {
  // Update to use RTCRayHit, which includes the RTCRay

  auto &ray = rayhit.ray;
  auto &hit = rayhit.hit;
  application ap;

  RT_APPLICATION_INIT(&ap);

  ap.a_rt_i = geom.rtip;
  ap.a_onehit = 1;
  ap.a_resource = &geom.resources[getCpuId()];

  VSET(ap.a_ray.r_pt, ray.org_x, ray.org_y, ray.org_z);
  VSET(ap.a_ray.r_dir, ray.dir_x, ray.dir_y, ray.dir_z);
  ap.a_ray.r_min = ray.tnear;
  ap.a_ray.r_max = ray.tfar;

  ap.a_hit = hitCallback;
  ap.a_miss = missCallback;
  ap.a_logoverlap = rt_silent_logoverlap;
  ap.a_uptr = &rayhit;

  // Reset hit info so misses cannot leak stale data.
  hit.geomID = RTC_INVALID_GEOMETRY_ID;
  hit.primID = RTC_INVALID_GEOMETRY_ID;

  auto didHit = rt_shootray(&ap);
  if (didHit) {
    hit.geomID = geom.geomID;
    hit.primID = 0;
  }
}

static void brlcadIntersect(const BRLCAD *geom_i, RTCRayHit &rayhit,
                            size_t item) {

  // Update to use RTCRayHit, which includes the RTCRay

  const BRLCAD &geom = geom_i[item];
  traceRay(geom, rayhit);
}

// static void brlcadBounds(void *geom_i, size_t item, RTCBounds &bounds_o) {
//   const auto &geom = ((const BRLCAD *)geom_i)[item];
//   bounds_o.lower_x = geom.bounds.lower.x;
//   bounds_o.lower_y = geom.bounds.lower.y;
//   bounds_o.lower_z = geom.bounds.lower.z;
//   bounds_o.upper_x = geom.bounds.upper.x;
//   bounds_o.upper_y = geom.bounds.upper.y;
//   bounds_o.upper_z = geom.bounds.upper.z;
// }

static void brlcadBounds(const struct RTCBoundsFunctionArguments *args) {
  // Updated bounds function
  const BRLCAD *geoms = (const BRLCAD *)args->geometryUserPtr;
  RTCBounds *bounds_o = args->bounds_o;
  const BRLCAD &geom = geoms[args->primID];
  bounds_o->lower_x = geom.p.x - geom.r;
  bounds_o->lower_y = geom.p.y - geom.r;
  bounds_o->lower_z = geom.p.z - geom.r;
  bounds_o->upper_x = geom.p.x + geom.r;
  bounds_o->upper_y = geom.p.y + geom.r;
  bounds_o->upper_z = geom.p.z + geom.r;
}

void BRLCAD::commit() {
  if (rtip)
    rt_free_rti(rtip);

  std::string filename = getParamString("filename");
  std::string objects = getParamString("objects");

  rtip = rt_dirbuild(filename.c_str(), nullptr, 0);

  const int nThreads = tasking::numTaskingThreads();

  std::cout << "nthreads is " << nThreads << std::endl;

  resources.resize(MAX_PSW);
  for (int i = 0; i < MAX_PSW; ++i)
    rt_init_resource(&resources[i], i, rtip);

  auto objNames = ospcommon::utility::split(objects, ',');
  for (const auto &obj : objNames) {
    const char *object = obj.c_str();
    rt_gettrees(rtip, 1, &object, 1);
  }

  rt_prep_parallel(rtip, tasking::numTaskingThreads());

  if (rtip == nullptr)
    throw std::runtime_error("BRL-CAD geometry requires an existing rt_i!");

  bounds.lower.x = rtip->mdl_min[0];
  bounds.lower.y = rtip->mdl_min[1];
  bounds.lower.z = rtip->mdl_min[2];
  bounds.upper.x = rtip->mdl_max[0];
  bounds.upper.y = rtip->mdl_max[1];
  bounds.upper.z = rtip->mdl_max[2];
}

} // namespace brl_cad
} // namespace ospray
