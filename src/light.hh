#ifndef LIGHT_HH
#define LIGHT_HH

#include "core.hh"

struct light_t
{
  vec3f position;
  vec3f ambient;
  vec3f diffuse;
  vec3f specular;
};

#endif