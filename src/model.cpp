#include "model.hh"

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <omp.h>

Model::Model(const std::string& path)
{
  loadModel(path);
}

void Model::loadModel(const std::string& path)
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

  spdlog::info("mNumMeshes={}", scene->mNumMeshes);

  _meshes.reserve(scene->mNumMeshes);
  
  //#pragma omp parallel for
  for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    spdlog::info("Loading mesh N°{} with mNumVertices={} ", i, mesh->mNumVertices);
    loadMesh(scene, mesh);
  }

  // for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  // {
  //   const auto mesh = _meshes[i];
  //   spdlog::info("mesh {}: indices.size={}  textures.size={}", i, mesh->indices().size(), mesh->textures().size());
  // }

}

void Model::loadMesh(const aiScene* scene, aiMesh* mesh)
{
  // mNumFaces  : tells us how many polygons exist
  // mNumIndices: number of indices in the polygon

  std::vector<vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture*> textures;

  vertices.reserve(mesh->mNumVertices);
  indices.reserve(mesh->mNumFaces * 3);

  // load vertices
  for (uint32_t i = 0 ; i < mesh->mNumVertices; i++) 
  {
    vertex_t vertex;

    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;
    vertex.normal.x   = mesh->mNormals[i].x;
    vertex.normal.y   = mesh->mNormals[i].y;
    vertex.normal.z   = mesh->mNormals[i].z;

    if(mesh->HasTextureCoords(0))
    {
      vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
      vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
    }

    vertices.push_back(vertex);
  }

  // load indices
  for (uint32_t i = 0 ; i < mesh->mNumFaces; i++) 
  {
    const aiFace& Face = mesh->mFaces[i];
    indices.push_back(Face.mIndices[0]);
    indices.push_back(Face.mIndices[1]);
    indices.push_back(Face.mIndices[2]);
  } 

  // load textures
  for (uint32_t i = 0 ; i < scene->mNumMaterials; i++)
  {
    const aiMaterial* material = scene->mMaterials[i];

    // 1. load diffuse maps
    loadMaterial(textures, material, aiTextureType_DIFFUSE);
    
    // 2. load specular maps
    loadMaterial(textures, material, aiTextureType_SPECULAR);

    // 3. load normal maps
    loadMaterial(textures, material, aiTextureType_NORMALS);
    
    // 4. load height maps
    loadMaterial(textures, material, aiTextureType_HEIGHT);
  }

  _meshes.push_back(new Mesh(vertices, indices, textures));
}

void Model::loadMaterial(std::vector<Texture*>& out, const aiMaterial* material, aiTextureType type)
{
  for(uint32_t i = 0; i < material->GetTextureCount(type); i++)
  {
    aiString path;
    material->GetTexture(type, i, &path);

    std::string texPath = "res/textures/";
    texPath.append(path.C_Str());
    out.push_back(new Texture(texPath));
  }
}

void Model::draw(Shader* shader)
{
  for(auto& mesh : _meshes)
    mesh->draw(shader);
}

void Model::destroy()
{
  for(auto& mesh : _meshes)
  {
    mesh->destroy();
    delete mesh;
  }
}