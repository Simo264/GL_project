#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "texture.hh"
#include "camera.hh"
#include "model.hh"

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
  Camera camera { vec3f(0.0f, 0.0f, 7.0f), vec3f(0.0f, 0.0f, -1.0f) };
   
   
  // render loop
  // ------------------------------------------------------------------------
  vec3f cubePos { 0.0f, 0.0f, 0.0f };
  mat4f model;
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
    const mat4f view       = camera.lookAtTarget(cubePos);
    const mat4f projection = perspective(radians(45.f), (WINDOW_WIDTH / WINDOW_HEIGTH), 0.1f, 100.0f);
    
    model = mat4f(1.0f);
    model = translate(model, cubePos);
    model = rotate(model, radians((float)glfwGetTime() * 20), vec3f(0.f, 1.f, 0.f) ); 
    shaderMesh.use();
    shaderMesh.setMat4f("view",       view);
    shaderMesh.setMat4f("projection", projection);
    shaderMesh.setMat4f("model",      model);
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


