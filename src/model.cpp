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

void Model::draw(Shader* shader)
{
  shader->setMat4f("model", _model);
  for(uint32_t i = 0; i < _numMeshes; i++)
  {
    auto& mesh = _meshPool[i];
    mesh.preDraw();
    mesh.draw();
    mesh.postDraw();
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
  _meshPool  = std::make_unique<Mesh3D[]>(_numMeshes);

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
  vertices.reserve(aimesh->mNumVertices * 8); // 8 -> position(3)+normals(3)+textcoord(2)
  indices.reserve(aimesh->mNumFaces * 3);     // 3 -> 3 vertices per triangle 

  // load vertices
  for (uint32_t i = 0 ; i < aimesh->mNumVertices; i++) 
  {
    aiVector3D& vertPos = aimesh->mVertices[i];
    aiVector3D& vertNor = aimesh->mNormals[i];
    aiVector3D& vertTc  = aimesh->mTextureCoords[0][i];
    
    vertices.insert(vertices.end(), { vertPos.x,vertPos.y,vertPos.z });
    vertices.insert(vertices.end(), { vertNor.x,vertNor.y,vertNor.z });
    vertices.insert(vertices.end(), { vertTc.x,vertTc.y });
  }

  // load indices
  for (uint32_t i = 0 ; i < aimesh->mNumFaces; i++) 
  {
    const aiFace& face = aimesh->mFaces[i];
    indices.insert(indices.end(), { face.mIndices[0],face.mIndices[1],face.mIndices[2] });
  }

  // load mesh objects sequentially on the heap
  GL::VAConfiguration config;
  config.pushAttribute(3); //layout=0 vec3 position
  config.pushAttribute(3); //layout=1 vec3 normals
  config.pushAttribute(2); //layout=2 vec2 textcoords
  
  Mesh3D& mesh = _meshPool[index];
  mesh.init(config, vertices, indices);

  // load textures
  const aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
  Texture2D* diffuse  = loadTexture(material, "diffuse");
  Texture2D* normal   = loadTexture(material, "normal");
  Texture2D* specular = loadTexture(material, "specular");
  mesh.diffuse  = diffuse;
  mesh.normal   = normal;
  mesh.specular = specular;
}

Texture2D* Model::loadTexture(const aiMaterial* material, const char* textureType)
{
  aiTextureType aiType = aiTextureType_NONE;
  if(strcmp(textureType, "diffuse") == 0)
    aiType = aiTextureType_DIFFUSE; 
  else if(strcmp(textureType, "normal") == 0)
    aiType = aiTextureType_NORMALS; 
  else if(strcmp(textureType, "specular") == 0)
    aiType = aiTextureType_SPECULAR; 


  if (material->GetTextureCount(aiType) <= 0)
    return nullptr; 
  
  aiString filename;
  if(material->GetTexture(aiType, 0, &filename) != AI_SUCCESS)
    return nullptr;

  string path = "assets/";
  path.append(filename.C_Str());

  Texture2D* texture = pool::TexturePool::getTexture(path);
  if(!texture)  
    texture = pool::TexturePool::loadTexture(path);
  
  return texture;
}

