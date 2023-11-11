#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "texture.hh"
#include "camera.hh"
#include "model.hh"
#include "light.hh"
#include "material.hh"

#include "spdlog/spdlog.h"

#define WINDOW_WIDTH 720.0f
#define WINDOW_HEIGTH 720.0f

int main()
{ 
  Window window;
  window.create(vec2u(WINDOW_WIDTH, WINDOW_HEIGTH), "OpenGL");
  window.setPosition(vec2i(200,200));

  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader shaderMesh { "shaders/mesh.vert","shaders/mesh.frag" };  


  // create model objects
  // ------------------------------------------------------------------------
  // Model modelObject("models/Backpack/Backpack.obj");
  Model modelObject("models/Crate/Crate.obj");
  vec3f modelPosition { 0.0f, 0.0f, 0.0f };

  // create camera object
  // ------------------------------------------------------------------------
  Camera camera { &modelPosition, 3.f };


  // light object
  // ------------------------------------------------------------------------
  light_t light { 
    .position = vec3f(0.2f, 0.2f, 0.2f),
    .ambient  = vec3f(0.2f, 0.2f, 0.2f),
    .diffuse  = vec3f(0.5f, 0.5f, 0.5f),
    .specular = vec3f(1.0f, 1.0f, 1.0f)
  };
  material_t material {
    .diffuse = 0,
    .normal = 1,
    .specular = 2,
    .shininess = 64.f 
  };

   
  // render loop
  // ------------------------------------------------------------------------
  
  mat4f model;
  mat4f projection = perspective(radians(45.f), (WINDOW_WIDTH / WINDOW_HEIGTH), 0.1f, 100.0f);
  while(window.loop())
  {
    // update delta time
    // ------------------------------------------------------------------------
    window.update();
    const double deltaTime = window.delta();

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processKeyboardInput(&window, deltaTime);


    // Render 
    // ------------------------------------------------------------------------
    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // model/view transformations
    const mat4f view  = camera.getViewMatrix();
    
    model = mat4f(1.0f);
    model = translate(model, modelPosition);
    // model = rotate(model, radians((float)glfwGetTime() * 20), vec3f(0.f, 1.f, 0.f)); 
    // model = scale(model, vec3f(0.50, 0.50, 0.50));
    shaderMesh.use();
    shaderMesh.setMat4f("view",       view);
    shaderMesh.setMat4f("projection", projection);
    shaderMesh.setMat4f("model",      model);

    // material properties
    shaderMesh.setInt("material.diffuse",     material.diffuse);
    shaderMesh.setInt("material.normal",      material.normal);
    shaderMesh.setInt("material.specular",    material.specular);
    shaderMesh.setFloat("material.shininess", material.shininess);

    // light properties
    shaderMesh.setVec3f("light.position", light.position);
    shaderMesh.setVec3f("light.ambient",  light.ambient);
    shaderMesh.setVec3f("light.diffuse",  light.diffuse);
    shaderMesh.setVec3f("light.specular", light.specular);
    shaderMesh.setVec3f("viewPos",        camera.position);

    modelObject.draw(&shaderMesh, GL_TRIANGLES);

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


