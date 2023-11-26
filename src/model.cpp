#include "model.hh"

#include "spdlog/spdlog.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "pool/texture_pool.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Model::Model(const string& path) : Actor()  
{
  loadModel(path);
}

void Model::draw(Shader* shader, uint32_t drawmode)
{
  shader->use();
  shader->setMat4f("model", _model);
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    auto& mesh = _meshPool.get()[i];
    mesh.draw(shader, drawmode);
  }
}

void Model::destroy()
{
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    auto& mesh = _meshPool[i];
    mesh.destroy();
  }

  _meshPool.reset();
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

void Model::loadModel(const string& path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.c_str(),  aiProcess_Triangulate | 
                                                          aiProcess_GenSmoothNormals | 
                                                          aiProcess_FlipUVs | 
                                                          aiProcess_JoinIdenticalVertices);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
  {
    spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
    return;
  }
  
  // Allocates enough memory to hold `_numMeshes` mesh objects sequentially  
  _numMeshes = scene->mNumMeshes;
  _meshPool  = make_unique<Mesh[]>(_numMeshes);

  spdlog::info("Loading meshes {}", _numMeshes);
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    aiMesh* aimesh = scene->mMeshes[i];
    loadMesh(i, scene, aimesh);
  }
  spdlog::info("Done!");

}

void Model::loadMesh(uint32_t index, const aiScene* scene, const aiMesh* aimesh)
{
  vector<Vertex>   vertices;
  vector<uint32_t> indices;

  vertices.reserve(aimesh->mNumVertices);
  indices.reserve(aimesh->mNumFaces * 3);

  // load vertices
  loadVertices(vertices, aimesh);

  // load indices
  loadIndices(indices, aimesh);

  // load meshe in array
  Mesh* mesh = new(&_meshPool[index]) Mesh(vertices, indices);

  // load textures
  const aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
  Texture* diffuse  = loadTexture(material, TextureType::DIFFUSE);
  Texture* normal   = loadTexture(material, TextureType::NORMAL);
  Texture* specular = loadTexture(material, TextureType::SPECULAR);
  mesh->diffuse  = diffuse;
  mesh->normal   = normal;
  mesh->specular = specular;
}

void Model::loadVertices(vector<Vertex>& out, const aiMesh* aimesh)
{
  for (uint32_t i = 0 ; i < aimesh->mNumVertices; i++) 
  {
    Vertex vertex;
    vertex.position.x = aimesh->mVertices[i].x;
    vertex.position.y = aimesh->mVertices[i].y;
    vertex.position.z = aimesh->mVertices[i].z;
    vertex.normal.x   = aimesh->mNormals[i].x;
    vertex.normal.y   = aimesh->mNormals[i].y;
    vertex.normal.z   = aimesh->mNormals[i].z;
    vertex.texCoord.x = aimesh->mTextureCoords[0][i].x;
    vertex.texCoord.y = aimesh->mTextureCoords[0][i].y;
    out.push_back(vertex);
  }
}

void Model::loadIndices(vector<uint32_t>& out,  const aiMesh* aimesh)
{
  // mNumFaces  : tells us how many polygons exist
  for (uint32_t i = 0 ; i < aimesh->mNumFaces; i++) 
  {
    const aiFace& Face = aimesh->mFaces[i];
    out.push_back(Face.mIndices[0]);
    out.push_back(Face.mIndices[1]);
    out.push_back(Face.mIndices[2]);
  } 
}

Texture* Model::loadTexture(const aiMaterial* material, const TextureType texType)
{
  aiTextureType aiType;
  switch (texType)
  {
  case TextureType::DIFFUSE:  aiType = aiTextureType_DIFFUSE; 
    break;
  case TextureType::NORMAL:   aiType = aiTextureType_NORMALS; 
    break;
  case TextureType::SPECULAR: aiType = aiTextureType_SPECULAR; 
    break;
  default:
    break;
  }

  if (material->GetTextureCount(aiType) <= 0)
    return nullptr; 
  
  aiString filename;
  if(material->GetTexture(aiType, 0, &filename) != AI_SUCCESS)
    return nullptr;

  string path = "assets/";
  path.append(filename.C_Str());
  path.erase(remove_if(path.begin(), path.end(), isspace), path.end()); // remove whitespaces

  Texture* texture = pool::TexturePool::getTexture(path);
  if(!texture)  
    texture = pool::TexturePool::loadTexture(path, texType, false);
  
  return texture;

#if 0
  for(uint32_t i = 0; i < material->GetTextureCount(aiType); i++)
  {
    aiString filename;
    material->GetTexture(aiType, i, &filename);

    string path = "assets/";
    path.append(filename.C_Str());

    Texture* texture = pool::TexturePool::getTexture(path);
    if(!texture)
    {
      texture = pool::TexturePool::loadTexture(path, texType, false);
      return texture;
    }
  }
  return nullptr;
#endif
}

