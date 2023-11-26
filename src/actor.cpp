#include "actor.hh"

Actor::Actor()
{
  _scaling     = mat4f(1.0f);
  _rotation    = mat4f(1.0f);
  _translation = mat4f(1.0f);
  
  updateModel();
}

void Actor::scale(vec3f size)
{
  _scaling = glm::scale(mat4f(1.0f), size);
  updateModel();
}

void Actor::translate(vec3f position)
{
  _translation = glm::translate(mat4f(1.0f), position);
  updateModel();
}

void Actor::rotate(float angle, vec3f axis)
{
  _rotation = glm::rotate(mat4f(1.0f), angle, axis);
  updateModel();
}

