#include "model.hh"

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <map>

static std::map<std::string, Texture*> GLOBAL_TEXTURES;

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

  for(uint32_t i = 0; i < scene->mNumMeshes; i++)
  {
    spdlog::info("Loading mesh °{}...", i);
    aiMesh* mesh = scene->mMeshes[i];
    _meshes.push_back(loadMesh(scene, mesh));
  }

  spdlog::info("Done!");
}

Mesh* Model::loadMesh(const aiScene* scene, const aiMesh* mesh)
{


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
  const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  loadTextures(textures, material, aiTextureType_DIFFUSE,  TextureType::TEX_DIFFUSE);
  loadTextures(textures, material, aiTextureType_NORMALS,  TextureType::TEX_NORMAL);
  loadTextures(textures, material, aiTextureType_SPECULAR, TextureType::TEX_SPECULAR);


  return new Mesh(vertices, indices, textures);
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
  // mNumFaces  : tells us how many polygons exist
  for (uint32_t i = 0 ; i < mesh->mNumFaces; i++) 
  {
    const aiFace& Face = mesh->mFaces[i];
    out.push_back(Face.mIndices[0]);
    out.push_back(Face.mIndices[1]);
    out.push_back(Face.mIndices[2]);
  } 
}

void Model::loadTextures(
  std::vector<Texture*>&  out, 
  const aiMaterial*       material, 
  const aiTextureType     aiType,
  const TextureType       texType
)
{
  for(uint32_t i = 0; i < material->GetTextureCount(aiType); i++)
  {
    aiString filename;
    material->GetTexture(aiType, i, &filename);

    std::string path = "res/textures/";
    path.append(filename.C_Str());

    Texture* texture = nullptr;

    auto it = GLOBAL_TEXTURES.find(path);
    if (it == GLOBAL_TEXTURES.end())
    {
      texture = new Texture(path, texType);
      GLOBAL_TEXTURES.insert(std::make_pair(path,texture));
    }
    else
    {
      texture = it->second;
    }

    out.push_back(texture);
  }
}

