#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"
#include "light.hh"

int main()
{ 
  Window window;
  window.create(vec2u(720, 720), "OpenGL");
  window.setPosition(vec2i(200,200));


  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader shaderMesh { "shaders/mesh.vert","shaders/mesh.frag" };  


  // create model objects
  // ------------------------------------------------------------------------
  Model modelCrate("assets/Crate/Crate.obj");
  Model modelCrate_2("assets/Crate/Crate.obj");
  vec3f pos = modelCrate_2.position();
  pos.z = 2.7f;
  modelCrate_2.setPosition(pos);


  // create camera object
  // ------------------------------------------------------------------------
  pos = modelCrate.position();
  Camera camera(&pos, 5.0f);


  // light object
  // ------------------------------------------------------------------------
  light_t light { 
    .direction = vec3f(-0.2f, -1.0f, -0.3f),
    .ambient   = vec3f(0.1f, 0.1f, 0.1f),
    .diffuse   = vec3f(0.1f, 0.1f, 0.1f),
    .specular  = vec3f(0.25f, 0.25f, 0.25f)
  };

   
  const mat4f projection = perspective(radians(45.f), (float)window.width()/(float)window.height(), 0.1f, 100.0f);
  
  // render loop
  // ------------------------------------------------------------------------
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
    
    shaderMesh.use();
    shaderMesh.setMat4f("view",       camera.getViewMatrix());
    shaderMesh.setMat4f("projection", projection);
    shaderMesh.setVec3f("viewPos",    camera.position);

    // light properties
    shaderMesh.setVec3f("light.direction", light.direction);
    shaderMesh.setVec3f("light.ambient",   light.ambient);
    shaderMesh.setVec3f("light.diffuse",   light.diffuse);
    shaderMesh.setVec3f("light.specular",  light.specular);
    
    modelCrate.draw(&shaderMesh, GL_TRIANGLES);
    modelCrate_2.draw(&shaderMesh, GL_TRIANGLES);

    const auto time = glfwGetTime() / 2;
    light.direction.x = glm::sin(time) * 10.0f;
    light.direction.z = glm::cos(time) * 10.0f;

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


