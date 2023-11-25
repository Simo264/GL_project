#ifndef MODEL_HH
#define MODEL_HH

#include "core.hh"

#include "mesh.hh"
#include "shader.hh"
#include "texture.hh"

class Model
{
public:
  Model(const string& path);
  ~Model() = default;

  void draw(Shader* shader, uint32_t drawmode = GL_TRIANGLES); // GL_TRIANGLES | GL_LINE_STRIP
  
  void destroy();

  vec3f position() const { return _position;}
  void  setPosition(vec3f newPos);
  
  vec3f size() const { return _size; }
  void  setSize(vec3f newSz);
  
private:
  vector<Mesh*> _meshes;

  mat4f _translationMatrix;
  mat4f _scalingMatrix;
  mat4f _rotationMatrix;

  mat4f _modelMatrix;

  vec3f _position;
  vec3f _size;

  void updateModelMatrix() { _modelMatrix = _translationMatrix * _rotationMatrix * _scalingMatrix; }

  void loadModel(const string& path);
  void loadMesh(const struct aiScene* scene, const struct aiMesh* aimesh);
  void loadVertices(vector<Vertex>& out, const struct aiMesh* aimesh);
  void loadIndices(vector<uint32_t>& out,  const struct aiMesh* aimesh);
  Texture* loadTexture(const struct aiMaterial* material, const TextureType texType);
};  

#endif