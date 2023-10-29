#include "model.hh"

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <omp.h>

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Model::Model(const std::string& path)
{
  loadModel(path);
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


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

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

  _meshes.reserve(scene->mNumMeshes);
  
  #pragma omp parallel for
  for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  {
    spdlog::info("Loading mesh °{}...", i);
    aiMesh* mesh = scene->mMeshes[i];
    loadMesh(scene, mesh);
  }

  spdlog::info("Done!");
}

void Model::loadMesh(const aiScene* scene, const aiMesh* mesh)
{
  (void) scene;

  // mNumFaces  : tells us how many polygons exist
  // mNumIndices: number of indices in the polygon

  std::vector<vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture*> textures;

  vertices.reserve(mesh->mNumVertices);
  indices.reserve(mesh->mNumFaces * 3);

  // load vertices
  loadVertices(vertices, mesh);

  // load indices
  loadIndices(indices, mesh);

  // load textures
  // loadTextures(textures, scene);
  
  _meshes.push_back(new Mesh(vertices, indices, textures));
}

void Model::loadVertices(std::vector<vertex_t>& out, const aiMesh* mesh)
{
  for (uint32_t i = 0 ; i < mesh->mNumVertices; i++) 
  {
    vertex_t vertex;
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

void Model::loadIndices(std::vector<uint32_t>& out,  const aiMesh* mesh)
{
  for (uint32_t i = 0 ; i < mesh->mNumFaces; i++) 
  {
    const aiFace& Face = mesh->mFaces[i];
    out.push_back(Face.mIndices[0]);
    out.push_back(Face.mIndices[1]);
    out.push_back(Face.mIndices[2]);
  } 
}

void Model::loadTextures(std::vector<Texture*>& out, const aiScene* scene)
{
  for (uint32_t i = 0 ; i < scene->mNumMaterials; i++)
  {
    const aiMaterial* material = scene->mMaterials[i];

    // 1. load diffuse maps
    loadMaterial(out, material, aiTextureType_DIFFUSE);
    
    // 2. load specular maps
    loadMaterial(out, material, aiTextureType_SPECULAR);

    // 3. load normal maps
    loadMaterial(out, material, aiTextureType_NORMALS);
    
    // 4. load height maps
    loadMaterial(out, material, aiTextureType_HEIGHT);
  }
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

