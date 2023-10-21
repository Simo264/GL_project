#include "model.hh"

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

Model::Model(const std::string& path)
{
  loadModel(path);
}

void Model::loadModel(const std::string& path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | 
                                                          aiProcess_GenSmoothNormals | 
                                                          aiProcess_FlipUVs | 
                                                          aiProcess_JoinIdenticalVertices);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
  {
    spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
    return;
  }

  _meshes.reserve(scene->mNumMeshes);
  for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    loadMesh(scene, mesh);
  } 

}

void Model::loadMesh(const aiScene* scene, aiMesh* mesh)
{
  // mNumFaces     = 12  (tells us how many polygons exist)
  // mNumIndices   = 3   (number of indices in the polygon, in our case triangles)

  std::vector<vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture*> textures;

  vertices.reserve(mesh->mNumVertices);
  indices.reserve(mesh->mNumFaces * 3);

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

  for (uint32_t i = 0 ; i < mesh->mNumFaces; i++) 
  {
    const aiFace& Face = mesh->mFaces[i];
    indices.push_back(Face.mIndices[0]);
    indices.push_back(Face.mIndices[1]);
    indices.push_back(Face.mIndices[2]);
  }

  for (uint32_t i = 0 ; i < scene->mNumMaterials; i++)
  {
    const aiMaterial* pMaterial = scene->mMaterials[i];
    spdlog::info("mesh->mMaterialIndex={}", mesh->mMaterialIndex);

    aiString Path;
    
    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
    {
      char texturePath[20] = "res/";
      strcat(texturePath, Path.C_Str());

      textures.push_back(new Texture(texturePath));
    }
  }

  _meshes.push_back(new Mesh(vertices, indices, textures));
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