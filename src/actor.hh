#ifndef ACTOR_HH
#define ACTOR_HH

#include "core.hh"

class Actor
{
public:
  Actor();
  virtual ~Actor() = default;

  void scale(vec3f size);
  void translate(vec3f position);
  void rotate(float angle, vec3f axis);

  const mat4f& model() const { return _model; }

protected:
  mat4f _scaling;
  mat4f _rotation;
  mat4f _translation;
  mat4f _model;

private: 
  void updateModel() { _model = _translation * _rotation * _scaling; }
};

#endif