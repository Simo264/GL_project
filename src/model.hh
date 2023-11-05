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

  void draw(Shader* shader, uint32_t drawmode); // GL_TRIANGLES | GL_LINE_STRIP
  
  void destroy();

private:
  vector<Mesh*> _meshes;

  void loadModel(const string& path);
  void loadMesh(const struct aiScene* scene, const struct aiMesh* mesh);

  void loadVertices(vector<vertex_t>& out, const struct aiMesh* mesh);
  void loadIndices(vector<uint32_t>& out,  const struct aiMesh* mesh);
  void loadTextures(
    vector<Texture*>& out, 
    const struct aiMaterial* material, 
    const TextureType texType
  );
};  

#endif