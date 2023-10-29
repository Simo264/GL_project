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
  Model modelObject("models/Backpack/Backpack.obj");
  

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
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 20), glm::vec3(0.f, 1.f, 0.f) ); 
    shaderMesh.use();
    shaderMesh.setMat4("view",       view);
    shaderMesh.setMat4("projection", projection);
    shaderMesh.setMat4("model",      model);
    modelObject.draw(&shaderMesh);

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


