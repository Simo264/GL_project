#include "mesh.hh"

#include "spdlog/spdlog.h"

#include <string>

Mesh::Mesh(std::vector<vertex_t> vertices, std::vector<uint32_t> indices, std::vector<Texture*> textures)
{
  _vertices = vertices;
  _indices  = indices;
  _textures = textures;

  setup();
}

Mesh::Mesh(const std::string& path)
{
  load(path);

  setup();
}

void Mesh::setup()
{
  _vertexBuffer   = std::make_unique<VertexBuffer>(_vertices.size(), _vertices.data());
  _elementBuffer  = std::make_unique<ElementBuffer>(_indices.size(), _indices.data());
  _vertexArray    = std::make_unique<VertexArray>(*_vertexBuffer.get());
  _vertexArray.get()->unbind();
}

void Mesh::load(const std::string& path)
{
  (void) path;

  Assimp::Importer Importer;
  const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | 
                                                              aiProcess_GenSmoothNormals | 
                                                              aiProcess_FlipUVs | 
                                                              aiProcess_JoinIdenticalVertices);

  const aiMesh* paiMesh = pScene->mMeshes[0];

  _vertices.reserve(paiMesh->mNumVertices);
  for (uint32_t i = 0 ; i < paiMesh->mNumVertices; i++) 
  {
    vertex_t vertex;

    vertex.position.x = paiMesh->mVertices[i].x;
    vertex.position.y = paiMesh->mVertices[i].y;
    vertex.position.z = paiMesh->mVertices[i].z;
    vertex.normal.x   = paiMesh->mNormals[i].x;
    vertex.normal.y   = paiMesh->mNormals[i].y;
    vertex.normal.z   = paiMesh->mNormals[i].z;

    if(paiMesh->HasTextureCoords(0))
    {
      vertex.texCoord.x = paiMesh->mTextureCoords[0][i].x;
      vertex.texCoord.y = paiMesh->mTextureCoords[0][i].y;
    }
    
    _vertices.push_back(vertex);
  }

  for (uint32_t i = 0 ; i < paiMesh->mNumFaces; i++) 
  {
    const aiFace& Face = paiMesh->mFaces[i];

    _indices.push_back(Face.mIndices[0]);
    _indices.push_back(Face.mIndices[1]);
    _indices.push_back(Face.mIndices[2]);
  }

  _textures.reserve(pScene->mNumMaterials);
  for (uint32_t i = 0 ; i < pScene->mNumMaterials; i++) 
  {
    const aiMaterial* pMaterial = pScene->mMaterials[i];
    aiString Path;
    
    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
    {
      char texturePath[20] = "res/";
      strcat(texturePath, Path.C_Str());

      _textures.push_back(new Texture(texturePath, TextureType::TEX_DIFFUSE, true));
    }
  }

}



void Mesh::draw(Shader& shader)
{
  (void) shader;

  for(uint32_t i = 0; i < _textures.size(); i++)
  {
    Texture::activeTextUnit(i);
    _textures[i]->bind();
    shader.setInt("texture_" + std::to_string(i+1) , i);
  }

  _vertexArray.get()->bind();
  _elementBuffer.get()->bind();

  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
  
  _elementBuffer.get()->unbind();
  _vertexArray.get()->unbind();
}

void Mesh::destroy()
{
  _vertexArray.get()->destroy();
  _vertexBuffer.get()->destroy();
  
  if(_elementBuffer)
    _elementBuffer.get()->destroy();
}