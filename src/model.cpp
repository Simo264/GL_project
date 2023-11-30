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
  shader->setMat4f("model", _model);
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    auto& mesh = _meshPool[i];
    mesh.draw(drawmode);
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
  const aiScene* scene = importer.ReadFile(path.c_str(),  
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
  {
    spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
    return;
  }
  
  // Allocates memory on the heap to hold `_numMeshes` mesh objects
  _numMeshes = scene->mNumMeshes;
  _meshPool  = std::make_unique<Mesh[]>(_numMeshes);

  //spdlog::info("Loading meshes {}", _numMeshes);
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    aiMesh* aimesh = scene->mMeshes[i];
    loadMesh(i, scene, aimesh);
  }
  //spdlog::info("Done!");

}

void Model::loadMesh(uint32_t index, const aiScene* scene, const aiMesh* aimesh)
{
  vector<float>     vertices;
  vector<uint32_t>  indices;
  
  spdlog::info("mNumVertices={}", aimesh->mNumVertices);
  spdlog::info("mNumFaces={}", aimesh->mNumFaces);

  vertices.reserve(aimesh->mNumVertices * 8); // 8 -> position(3)+normals(3)+textcoord(2)
  indices.reserve(aimesh->mNumFaces * 3);     // 3 -> 3 vertices per triangle 

  int iVert = 0;
  int iInd  = 0;

  // load vertices
  for (uint32_t i = 0 ; i < aimesh->mNumVertices; i++) 
  {
    aiVector3D& vertPos = aimesh->mVertices[i];
    aiVector3D& vertNor = aimesh->mNormals[i];
    aiVector3D& vertTc  = aimesh->mTextureCoords[0][i];
    vertices[iVert++] = vertPos.x;
    vertices[iVert++] = vertPos.y;
    vertices[iVert++] = vertPos.z;
    vertices[iVert++] = vertNor.x;
    vertices[iVert++] = vertNor.y;
    vertices[iVert++] = vertNor.z;
    vertices[iVert++] = vertTc.x;
    vertices[iVert++] = vertTc.y;
  }

  // load indices
  for (uint32_t i = 0 ; i < aimesh->mNumFaces; i++) 
  {
    const aiFace& face = aimesh->mFaces[i];
    indices[iInd++] = face.mIndices[0];
    indices[iInd++] = face.mIndices[1];
    indices[iInd++] = face.mIndices[2];
  }

  spdlog::info("(Model) vertices.size={}", vertices.size());
  spdlog::info("(Model) indices.size={}", indices.size());

  // load mesh objects sequentially on the heap
  Mesh& mesh = _meshPool[index];
  mesh.init(vertices, indices);
  



  // load textures
  const aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
  Texture* diffuse  = loadTexture(material, TextureType::DIFFUSE);
  Texture* normal   = loadTexture(material, TextureType::NORMAL);
  Texture* specular = loadTexture(material, TextureType::SPECULAR);
  mesh.diffuse  = diffuse;
  mesh.normal   = normal;
  mesh.specular = specular;
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

