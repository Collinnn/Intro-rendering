#include <iostream>
#include <optix_world.h>
#include "my_glut.h"
#include "Camera.h"

using namespace optix;

void Camera::set(const float3& eye_point, const float3& view_point, const float3& up_vector, float camera_constant)
{
  eye = eye_point;
  lookat = view_point;
  up = up_vector;
  cam_const = camera_constant;
  

  // Compute camera coordinate frame (image plane normal and basis).
  //
  // Relevant data fields that are available (see Camera.h)
  // ip_normal  (the viewing direction and the 3rd basis vector, v)
  // ip_xaxis   (the 1st basis vector, b_1)
  // ip_yaxis   (the 2nd basis vector, b_2)
  //
  // Hint: The OptiX math library has a function normalize(v) which returns
  //       the vector v normalized and a function cross(v, w) which returns
  //       the cross product of the vectors v and w.
  


  // Assume that the height and the width of the film is 1.
  // With this information, use the pinhole camera model to compute
  // the field of view (fov) in degrees.
  //
  // Relevant data fields that are available (see Camera.h)
  // camera_constant  (the camera constant, d)
  // fov              (the vertical field of view of the camera, phi)
  //
  // Hints: (a) The inverse tangent function is called atan(x).               
  //        (b) The OptiX math library has a constant for 1/pi called M_1_PIf.
  //        (c) The following constant is the field of view that you should   
  //            get with the default scene (if you set a breakpoint and run   
  //            in Debug mode, you can use it to check your code).            
  
  double const height = 1.0 , const width = 1.0;

  //These does nothing with the height, width at 1. However it's nice to know 
  //ip_xaxis = (ip_xaxis + 0.5)*(1.0/height) - (0.5 * (width / height));      
  //ip_yaxis = (ip_yaxis + 0.5)*(1.0/height) - 0.5;                           
  ////////////////////////////////////////////////////////////////////////////
  
  //Change of basis matrix
  ip_normal = (lookat - eye) / normalize(lookat - eye);
  ip_xaxis = cross(ip_normal, up)/(normalize(cross(ip_normal,up_vector)));
  ip_yaxis = cross(ip_xaxis, ip_normal);
  fov = 2 * atan(1.0 / cam_const); //field of view
}

/// Get direction of viewing ray from image coords.
float3 Camera::get_ray_dir(const float2& coords) const
{
  // Given the image plane coordinates, compute the normalized ray
  // direction by a change of basis. THIS Might not be done correctly
  return make_float3(((2*(coords.x+0.5))/1)-1,(2*(coords.y+0.5)/1)-1,cam_const);
}

/// Return the ray corresponding to a set of image coords
Ray Camera::get_ray(const float2& coords) const
{
  // Use the function get_ray_dir(...) to construct a ray starting at
  // the eye and going through the position in the image plane given
  // by the image plane coordinates received as argument.
  //
  // Hint: You can set the ray type to 0 as the framework currently
  //       does not use different ray types.d
  return Ray(make_float3(coords,0.0),get_ray_dir(coords),0,0.0,0.0); //This is probaly not right
}

// OpenGL

void Camera::glSetPerspective(unsigned int width, unsigned int height) const
{
  GLdouble aspect = width/static_cast<float>(height);    

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, aspect, cam_const*NEAR_PLANE, FAR_PLANE);

  glMatrixMode(GL_MODELVIEW);
}

void Camera::glSetCamera() const
{
  gluLookAt(eye.x,   eye.y,   eye.z, 
	          lookat.x, lookat.y, lookat.z, 
	          up.x,    up.y,    up.z);
}

