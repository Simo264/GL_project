#ifndef LIGHT_HH
#define LIGHT_HH

#include "core.hh"

struct light_t
{
  vec3f position; 
  vec3f direction;
  
  vec3f ambient;
  vec3f diffuse;
  vec3f specular;

  // implementing attenuation
  const float constant = 1.f;
  float linear;
  float quadratic;
};

#endif