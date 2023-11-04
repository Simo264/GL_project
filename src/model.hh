#ifndef MODEL_HH
#define MODEL_HH

#include "core.hh"

#include "mesh.hh"
#include "shader.hh"
#include "texture.hh"

#include "assimp/scene.h"

class Model
{

public:
  Model(const string& path);
  ~Model() = default;

  void draw(Shader* shader);	
  
  void destroy();

private:
  vector<Mesh*> _meshes;

  void loadModel(const string& path);
  Mesh* loadMesh(const aiScene* scene, const aiMesh* mesh);

  void loadVertices(vector<vertex_t>& out, const aiMesh* mesh);
  void loadIndices(vector<uint32_t>& out,  const aiMesh* mesh);
  void loadTextures(
    vector<Texture*>& out, 
    const aiMaterial* material, 
    const aiTextureType aiType, 
    const TextureType texType
  );
};  

#endif