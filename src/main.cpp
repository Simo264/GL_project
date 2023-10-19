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

void loadMesh(const std::string& filename);
void initFromScene(const aiScene* pScene, const std::string& filename);

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
  Texture texContainer {"res/crate.jpg", TextureType::TEX_DIFFUSE, true};
  

  // create mesh object
  // ------------------------------------------------------------------------
  loadMesh("res/Crate.obj");
  
  std::vector<vertex_t> vertices  = {
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f}),
    vertex_t({ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f}),
    vertex_t({-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
    vertex_t({-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0})
  };
  std::vector<uint32_t> indices   = { };
  std::vector<Texture>  textures  = { texContainer };
  
  Mesh cubeMesh( vertices, indices, textures );



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
  texContainer.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
  return 0;
}

void loadMesh(const std::string& filename)
{
  Assimp::Importer Importer;
  const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | 
                                                              aiProcess_GenSmoothNormals | 
                                                              aiProcess_FlipUVs | 
                                                              aiProcess_JoinIdenticalVertices);

  if(pScene)
    initFromScene(pScene, filename);
  else
    spdlog::error("pScene is nullptr");
}


void initFromScene(const aiScene* pScene, const std::string& filename)
{
  (void) filename;

  spdlog::info("pScene->mNumMeshes={}", pScene->mNumMeshes);
  spdlog::info("pScene->mNumMaterials={}", pScene->mNumMaterials);
}

