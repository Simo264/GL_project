#ifndef MODEL_HH
#define MODEL_HH

#include "mesh.hh"
#include "shader.hh"

#include "assimp/scene.h"

#include <string>
#include <vector>

class Model
{

public:
  Model(const std::string& path);
  ~Model() = default;

  void draw(Shader* shader);	
  
  void destroy();

private:
  std::vector<Mesh*> _meshes;

  void loadModel(const std::string& path);
  Mesh* loadMesh(const aiScene* scene, const aiMesh* mesh);

  void loadVertices(std::vector<vertex_t>& out, const aiMesh* mesh);
  void loadIndices(std::vector<uint32_t>& out,  const aiMesh* mesh);
  void loadTextures(
    std::vector<Texture*>& out, 
    const aiMaterial* material, 
    const aiTextureType aiType, 
    const TextureType texType
    );
};  

#endif