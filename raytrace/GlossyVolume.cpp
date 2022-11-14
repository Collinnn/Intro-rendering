// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "GlossyVolume.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 GlossyVolume::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Compute the specular part of the glossy shader and attenuate it
  // by the transmittance of the material if the ray is inside (as in
  // the volume shader).
	float3 rho_s = get_specular(hit);
	float shine = get_shininess(hit);

	float3 acc = make_float3(0.f);

	for (int i = 0; i < lights.size(); i++) {
		float3 dir;
		float3 L;
		if (lights[i]->sample(hit.position, dir, L) ){
			float dotted = optix::dot(hit.shading_normal,dir);
			if (dotted>0) {
				int n = (int)shine;
				float3 V = optix::reflect(-dir, hit.shading_normal);
				float a = optix::dot(-r.direction, V);
				if (a < 0.0) {
					a = 0;
				}
				else {
					a = int_pow(a, n);
				}
				acc += L * rho_s * a;
			}
		}
	}


	return Volume::shade(r, hit, emit) + 0.1 * acc;
}
