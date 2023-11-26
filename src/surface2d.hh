#ifndef SURFACE_2D_HH
#define SURFACE_2D_HH

#include "core.hh"
#include "mesh.hh"
#include "actor.hh"

class Surface2D : public Mesh, public Actor
{
public:
  Surface2D();
  ~Surface2D();
};

#endif