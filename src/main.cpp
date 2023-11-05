#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "texture.hh"
#include "camera.hh"
#include "model.hh"

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
  Model modelObject("models/Backpack/Backpack.obj");
  

  // create camera object
  // ------------------------------------------------------------------------
  Camera camera { vec3f(0.0f, 0.0f, 7.0f), vec3f(0.0f, 0.0f, -1.0f) };

   
  // render loop
  // ------------------------------------------------------------------------
  vec3f targetPosition { 0.0f, 0.0f, 0.0f };
  mat4f model;
  mat4f projection = perspective(radians(45.f), (WINDOW_WIDTH / WINDOW_HEIGTH), 0.1f, 100.0f);
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
    const mat4f view  = camera.lookAtTarget(targetPosition);
    
    
    model = mat4f(1.0f);
    model = translate(model, targetPosition);
    model = rotate(model, radians((float)glfwGetTime() * 20), vec3f(0.f, 1.f, 0.f)); 
    model = scale(model, vec3f(0.50, 0.50, 0.50));
    shaderMesh.use();
    shaderMesh.setMat4f("view",       view);
    shaderMesh.setMat4f("projection", projection);
    shaderMesh.setMat4f("model",      model);

    // material properties
    shaderMesh.setInt("material.diffuse", 0);
    shaderMesh.setInt("material.specular", 1);
    shaderMesh.setFloat("material.shininess", 64.0f);

    // light properties
    static const vec3f lightPosition { 0.2f, 0.2f, 0.2f };
    static const vec3f lightAmbient  { 0.2f, 0.2f, 0.2f };
    static const vec3f lightDiffuse  { 0.5f, 0.5f, 0.5f };
    static const vec3f lightSpecular { 1.0f, 1.0f, 1.0f };
    shaderMesh.setVec3f("light.position",  lightPosition);
    shaderMesh.setVec3f("light.ambient",  lightAmbient);
    shaderMesh.setVec3f("light.diffuse",  lightDiffuse);
    shaderMesh.setVec3f("light.specular", lightSpecular);
    shaderMesh.setVec3f("viewPos", camera.position);

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


