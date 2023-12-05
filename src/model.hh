#ifndef MODEL_HH
#define MODEL_HH

#include "core.hh"

#include "mesh3d.hh"
#include "shader.hh"
#include "texture2d.hh"
#include "actor.hh"

class Model : public Actor
{
public:
  Model(const string& path);
  ~Model() = default;
  
  Model(const Model&) = delete;            // delete copy constructor
  Model& operator=(const Model&) = delete; // delete assign op

  void draw(Shader* shader); // GL_TRIANGLES | GL_LINE_STRIP
  
  void destroy();

  const uint32_t& numMeshes() const { return _numMeshes; }
  Mesh3D& getMesh(uint32_t i) { return _meshPool[i]; }
  
private:
  unique_ptr<Mesh3D[]> _meshPool;
  uint32_t             _numMeshes;

  void loadModel(const string& path);
  void loadMesh(uint32_t index, const struct aiScene* scene, const struct aiMesh* aimesh);
  Texture2D* loadTexture(const struct aiMaterial* material, const char* textureType);
};  

#endif