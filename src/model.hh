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
  void loadMesh(const aiScene* scene, aiMesh* mesh);
};  

#endif