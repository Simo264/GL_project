#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "window.hh"
#include "vertex.hh"
#include "shader.hh"
#include "texture.hh"
#include "camera.hh"
#include "mesh.hh"
#include "model.hh"

#include <string>

#define WINDOW_WIDTH 720.0f
#define WINDOW_HEIGTH 720.0f

Mesh loadMesh(const std::string& path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.c_str(),  aiProcess_Triangulate | 
                                                          aiProcess_GenSmoothNormals | 
                                                          aiProcess_FlipUVs | 
                                                          aiProcess_JoinIdenticalVertices);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
  {
    spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
  }

  std::vector<vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture*> textures;


  auto mesh = scene->mMeshes[0];

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
    const aiMaterial* pMaterial = scene->mMaterials[i];

    aiString Path;
    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
    {
      std::string texPath = "res/textures/";
      texPath.append(Path.C_Str());
      textures.push_back(new Texture(texPath));
    }
  }

  return Mesh(vertices, indices, textures);
}

int main()
{ 
  Window window;
  window.create("OpenGL", WINDOW_WIDTH, WINDOW_HEIGTH);
  window.setPosition(200,200);


  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader shaderMesh { "shaders/mesh.vert","shaders/mesh.frag" };
  

  // create model objects
  // ------------------------------------------------------------------------
  Mesh mesh = loadMesh("models/Crate/Crate.obj");
  //Model modelObject("models/Backpack/backpack.obj");
  //Model modelObject("models/Crate/Crate.obj");


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
    (void) deltaTime;

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    // camera.processKeyboardInput(window, deltaTime); 
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
    // model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.f));
    // model = glm::rotate(model, glm::radians((float)glfwGetTime() * 20), glm::vec3(0.f, 1.f, 0.f) ); 
  
    shaderMesh.use();
    shaderMesh.setMat4("view", view);
    shaderMesh.setMat4("projection", projection);
    shaderMesh.setMat4("model", model);
    mesh.draw(&shaderMesh);

    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  shaderMesh.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
  return 0;
}


