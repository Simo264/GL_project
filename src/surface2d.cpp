#include "surface2d.hh"

static vector<GL::Vertex> vertices = {
  GL::Vertex{vec3f(1.0f,   1.0f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(1.0f, 1.0f)}, // top right
  GL::Vertex{vec3f(1.0f,  -1.0f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(1.0f, 0.0f)}, // bottom right
  GL::Vertex{vec3f(-1.0f, -1.0f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(0.0f, 0.0f)}, // bottom left
  GL::Vertex{vec3f(-1.0f,  1.0f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(0.0f, 1.0f)}, // top left 
};
static vector<uint32_t> indices = {0,1,3, 1,2,3};

Surface2D::Surface2D() : Mesh(vertices, indices), Actor()
{ }

Surface2D::~Surface2D()
{ }

