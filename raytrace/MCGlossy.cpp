// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "mt_random.h"
#include "sampler.h"
#include "HitInfo.h"
#include "MCGlossy.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 MCGlossy::shade(const Ray& r, HitInfo& hit, bool emit) const
{
    if (hit.trace_depth >= max_depth) {
        return make_float3(0.0f);
    }

    const float3 rho_d = get_diffuse(hit);
    const double probability = (rho_d.x + rho_d.y + rho_d.z) / 3.0f; //Red, green, blue (the 3 angles)
    float3 result = make_float3(0.0f);
    if (mt_random() < probability) {
        const float3 out_dir = sample_cosine_weighted(hit.geometric_normal);
        Ray out = Ray();
        HitInfo ref_hit;
        ref_hit.trace_depth = hit.trace_depth + 1;
        out.origin = hit.position;
        out.direction = out_dir;
        out.tmin = 1e-4f;
        out.tmax = RT_DEFAULT_MAX;
        tracer->trace_to_closest(out, ref_hit);
        result = shade_new_ray(out, ref_hit) * rho_d;
    }//In the case of absorption do nothing, therefore no else case.
    result += Lambertian::shade(r, hit, emit) * rho_d;
    result += Emission::shade(r, hit, emit);
    return result;

    // Implement a path tracing shader here.
    //
    // Input:  r          (the ray that hit the material)
    //         hit        (info about the ray-surface intersection)
    //         emit       (passed on to Emission::shade)
    //
    // Return: radiance reflected to where the ray was coming from
    //
    // Relevant data fields that are available (see Mirror.h and HitInfo.h):
    // max_depth          (maximum trace depth)
    // tracer             (pointer to ray tracer)
    // hit.trace_depth    (number of surface interactions previously suffered by the ray)
    //
    // Hint: Use the function shade_new_ray(...) to pass a newly traced ray to
    //       the shader for the surface it hit.

}
