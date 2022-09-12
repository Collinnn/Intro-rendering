// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Sphere.h"

using namespace optix;

bool Sphere::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
  // Implement ray-sphere intersection here.
  //
  // Input:  r                    (the ray to be checked for intersection)
  //         prim_idx             (index of the primitive element in a collection, not used here)
  //
  // Output: hit.has_hit          (set true if the ray intersects the sphere)
  //         hit.dist             (distance from the ray origin to the intersection point)
  //         hit.position         (coordinates of the intersection point)
  //         hit.geometric_normal (the normalized normal of the sphere)
  //         hit.shading_normal   (the normalized normal of the sphere)
  //         hit.material         (pointer to the material of the sphere)
  //        (hit.texcoord)        (texture coordinates of intersection point, not needed for Week 1)
  //
  // Return: True if the ray intersects the sphere, false otherwise
  //
  // Relevant data fields that are available (see Sphere.h and OptiX math library reference)
  // r.origin                     (ray origin)
  // r.direction                  (ray direction)
  // r.tmin                       (minimum intersection distance allowed)
  // r.tmax                       (maximum intersection distance allowed)
  // center                       (sphere center)
  // radius                       (sphere radius)
  // material                     (material of the sphere)
  //
  // Hints: (a) The square root function is called sqrt(x).
  //        (b) There is no need to handle the case where the 
  //            discriminant is zero separately.
	float3 orio_to_center = r.origin - center;
	float b = dot(orio_to_center, r.direction);
	float c = dot(orio_to_center, orio_to_center) - (radius*radius);
	
	//intersection points t1,t2
	float d = (b * b)-c;
	if (d) return false;
	float t1 = -b - sqrt(d - c);
	float t2 = -b + sqrt(d - c);
	if (t1 <= r.tmax && t1 >= r.tmin && t2 <= r.tmax && t2 >= r.tmin) {
		hit.has_hit = true;
		hit.position = r.origin + hit.dist * r.direction;
		hit.geometric_normal = normalize(hit.position - center);
		hit.shading_normal = hit.geometric_normal;
		hit.material = &material;
		return true;
	}

	return true;
}

void Sphere::transform(const Matrix4x4& m)
{
  float3 radius_vec = make_float3(radius, 0.0f, 0.0f) + center;
  radius_vec = make_float3(m*make_float4(radius_vec, 1.0f));
  center = make_float3(m*make_float4(center, 1.0f)); 
  // The radius is scaled by the X scaling factor.
  // Not ideal, but the best we can do without elipsoids
  radius_vec -= center;
  radius = length(radius_vec);  
}

Aabb Sphere::compute_bbox() const
{
  Aabb bbox;
  bbox.include(center - make_float3(radius, 0.0f, 0.0f));
  bbox.include(center + make_float3(radius, 0.0f, 0.0f));
  bbox.include(center - make_float3(0.0f, radius, 0.0f));
  bbox.include(center + make_float3(0.0f, radius, 0.0f));
  bbox.include(center - make_float3(0.0f, 0.0f, radius));
  bbox.include(center + make_float3(0.0f, 0.0f, radius));
  return bbox;
}
