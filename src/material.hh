#ifndef MATERIAL_HH
#define MATERIAL_HH

#include "core.hh"

struct material_t
{
  uint32_t diffuse;
  uint32_t normal;
  uint32_t specular;

  float shininess;
};

#endif