#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "vertex.hh"
#include "window.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"
#include "texture.hh"
#include "camera.hh"
#include "mesh.hh"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#define WINDOW_WIDTH 720.0f
#define WINDOW_HEIGTH 720.0f

std::vector<vertex_t> vertices;
std::vector<uint32_t> indices;
std::vector<Texture*> textures;

void loadMesh(const std::string& path);

int main()
{ 
  Window window;
  window.create("OpenGL", WINDOW_WIDTH, WINDOW_HEIGTH);
  window.setPosition(200,200);


  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader shader { "shaders/texture.vert","shaders/texture.frag" };
  

  // create texture object
  // ------------------------------------------------------------------------
  Texture texture {"res/crate.jpg", TextureType::TEX_DIFFUSE, true};
  

  // create mesh object
  // ------------------------------------------------------------------------
  // std::vector<vertex_t> vertices = {
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ),
  //   vertex_t( {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ),
  //   vertex_t( {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ),
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ),
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ),
  //   vertex_t( {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ), 
  //   vertex_t( { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ), 
  //   vertex_t( { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  //   vertex_t( {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } ), 
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ),
  //   vertex_t( {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } ),
  //   vertex_t( { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } ),
  //   vertex_t( { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ), 
  // };
  // std::vector<uint32_t> indices   = { };
  // std::vector<Texture*> textures  = { &texture };
  loadMesh("res/Crate/Crate.obj");
  Mesh cubeMesh(vertices, indices, textures );

  // create camera object
  // ------------------------------------------------------------------------
  Camera camera { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f) };
   
  // render loop
  // ------------------------------------------------------------------------
  glm::vec3 cubePos = { 0.0f, 0.0f, 0.0f };
  glm::mat4 model;
  while(window.loop())
  {
    // update delta time
    // ------------------------------------------------------------------------
    window.update();
    const double deltaTime = window.delta();

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processKeyboardInput(window, deltaTime); 
    // camera.processMouseMovement(window); 

    // Render 
    // ------------------------------------------------------------------------
    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // view/projection transformations
    const glm::mat4 view       = camera.lookAt(cubePos);
    const glm::mat4 projection = glm::perspective(glm::radians(45.f), (WINDOW_WIDTH / WINDOW_HEIGTH), 0.1f, 100.0f);
    
    // draw the mesh
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", model);
    cubeMesh.draw(shader);

    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  shader.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
  return 0;
}

void loadMesh(const std::string& path)
{
  (void) path;

  Assimp::Importer Importer;
  const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | 
                                                          aiProcess_GenSmoothNormals | 
                                                          aiProcess_FlipUVs | 
                                                          aiProcess_JoinIdenticalVertices);

  // Initialize the meshes in the scene one by one
  // mNumMeshes    = 1
  // mNumVertices  = 20
  // mNumFaces     = 12  (tells us how many polygons exist)
  // mNumIndices   = 3   (number of indices in the polygon)
  // mNumMaterials = 2   (holds the number of materials)

  const aiMesh* paiMesh = pScene->mMeshes[0]; 

  vertices.reserve(paiMesh->mNumVertices);
  for (uint32_t i = 0; i < paiMesh->mNumVertices; i++) 
  {
    vertex_t vertex;

    vertex.position.x = paiMesh->mVertices[i].x;
    vertex.position.y = paiMesh->mVertices[i].y;
    vertex.position.z = paiMesh->mVertices[i].z;

    vertex.normal.x = paiMesh->mNormals[i].x;
    vertex.normal.y = paiMesh->mNormals[i].y;
    vertex.normal.z = paiMesh->mNormals[i].z;

    vertex.texCoord.x = paiMesh->mTextureCoords[0][i].x;
    vertex.texCoord.y = paiMesh->mTextureCoords[0][i].y;

    vertices.push_back(vertex);
  }

  indices.reserve(paiMesh->mNumFaces*3);
  for (uint32_t i = 0; i < paiMesh->mNumFaces; i++)
  {
    const aiFace& Face = paiMesh->mFaces[i];
    indices.push_back(Face.mIndices[0]);
    indices.push_back(Face.mIndices[1]);
    indices.push_back(Face.mIndices[2]); 
  }

  for (uint32_t i = 0; i < pScene->mNumMaterials; i++) 
  {
    const aiMaterial* pMaterial = pScene->mMaterials[i];
    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
      aiString Path;
      if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
      {
        std::string texturePath = std::string("res/") + std::string(Path.C_Str()) ;
        textures.push_back(new Texture(texturePath, TextureType::TEX_DIFFUSE, true));
      }
    }
  }

}


