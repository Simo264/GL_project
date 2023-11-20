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

Model::Model(const string& path)
{
  loadModel(path);

  _position = vec3f(0.f,0.f,0.f); // default on origin
  _size     = vec3f(1.f,1.f,1.f); // default size

  _translationMatrix = translate(mat4f(1.f), _position);
  _scalingMatrix     = scale(mat4f(1.f), _size);
  _rotationMatrix    = mat4f(1.f); // no rotation
  
  updateModelMatrix();
}

void Model::draw(Shader* shader, uint32_t drawmode)
{
  shader->setMat4f("model", _modelMatrix);
  for(auto& mesh : _meshes)
    mesh->draw(shader, drawmode);
}

void Model::destroy()
{
  for(auto& mesh : _meshes)
  {
    mesh->destroy();
    delete mesh;
  }
}

void Model::setPosition(vec3f newPos)
{
  _translationMatrix = translate(mat4f(1.f), newPos);
  updateModelMatrix();
}

void Model::setSize(vec3f newSz)
{
  _scalingMatrix = scale(mat4f(1.f), newSz);
  updateModelMatrix();
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

  _meshes.reserve(scene->mNumMeshes);

  //spdlog::info("Loading meshes {}", scene->mNumMeshes);
  for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    loadMesh(scene, mesh);
  }
  //spdlog::info("Done!");
}

void Model::loadMesh(const aiScene* scene, const aiMesh* mesh)
{
  (void) scene;

  vector<Vertex> vertices;
  vector<uint32_t> indices;
  vector<Texture*> textures;

  vertices.reserve(mesh->mNumVertices);
  indices.reserve(mesh->mNumFaces * 3);

  // load vertices
  loadVertices(vertices, mesh);

  // load indices
  loadIndices(indices, mesh);

  // load textures
  const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  loadTextures(textures, material, TextureType::TEX_DIFFUSE);
  loadTextures(textures, material, TextureType::TEX_NORMAL);
  loadTextures(textures, material, TextureType::TEX_SPECULAR);

  _meshes.push_back(new Mesh(vertices, indices, textures));
}

void Model::loadVertices(vector<Vertex>& out, const aiMesh* mesh)
{
  for (uint32_t i = 0 ; i < mesh->mNumVertices; i++) 
  {
    Vertex vertex;
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;
    vertex.normal.x   = mesh->mNormals[i].x;
    vertex.normal.y   = mesh->mNormals[i].y;
    vertex.normal.z   = mesh->mNormals[i].z;
    vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
    vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
    out.push_back(vertex);
  }
}

void Model::loadIndices(vector<uint32_t>& out,  const aiMesh* mesh)
{
  // mNumFaces  : tells us how many polygons exist
  for (uint32_t i = 0 ; i < mesh->mNumFaces; i++) 
  {
    const aiFace& Face = mesh->mFaces[i];
    out.push_back(Face.mIndices[0]);
    out.push_back(Face.mIndices[1]);
    out.push_back(Face.mIndices[2]);
  } 
}

void Model::loadTextures(vector<Texture*>& out, const aiMaterial* material, const TextureType texType)
{
  aiTextureType aiType;
  if(texType == TextureType::TEX_DIFFUSE)
    aiType = aiTextureType_DIFFUSE; 
  else if(texType == TextureType::TEX_NORMAL)
    aiType = aiTextureType_NORMALS; 
  else if(texType == TextureType::TEX_SPECULAR)
    aiType = aiTextureType_SPECULAR; 


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
    }

    out.push_back(texture);
  }
}

