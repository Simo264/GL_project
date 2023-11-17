#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"

#include "lighting/directional_light.hh"
#include "lighting/point_light.hh"

#include "pool/shader_pool.hh"
#include "pool/texture_pool.hh"

#include "spdlog/spdlog.h"


int main()
{ 
  Window window;
  window.create(vec2u(720, 720), "OpenGL");
  window.setPosition(vec2i(200,200));

  pool::ShaderPool::initialize();
  pool::TexturePool::initialize();


  // load shaders
  // ------------------------------------------------------------------------
  auto shaderScene = pool::ShaderPool::loadShader("shaderMesh", "shaders/mesh.vert","shaders/mesh.frag");
  

  // create model objects
  // ------------------------------------------------------------------------
  // Model modelFloor("assets/Floor/Floor.obj");
  // modelFloor.setPosition(vec3f(0.0f, -1.0f, 0.0f));
  // modelFloor.setSize(vec3f(10.0f, 1.0f, 10.0f));

  Model modelCrate("assets/Crate/Crate.obj");
  Model modelCrate_2("assets/Crate/Crate.obj");
  modelCrate_2.setPosition(vec3f(0.0f, 0.0f, 2.70f));


  // create camera object
  // ------------------------------------------------------------------------
  vec3f pos = modelCrate.position();
  Camera camera(&pos, 5.0f);


  // light object
  // ------------------------------------------------------------------------
  lighting::DirectionalLight dirLight; (void) dirLight;
  lighting::PointLight pointLight; (void) pointLight;

  const mat4f projection = perspective(radians(45.f), (float)window.width()/(float)window.height(), 0.1f, 100.0f);
  
  #if MEASURE_SPEED 
  double lastTime = glfwGetTime();
  int nbFrames = 0;
  #endif
  
  // render loop
  // ------------------------------------------------------------------------
  while(window.loop())
  {
    // update delta time
    // ------------------------------------------------------------------------
    window.update();
    const double deltaTime = window.delta();

    #if MEASURE_SPEED 
    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
      // printf and reset timer
      printf("%f ms/frame\n", 1000.0/double(nbFrames));
      nbFrames = 0;
      lastTime += 1.0;
    }
    #endif

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processKeyboardInput(&window, deltaTime);


    // Render 
    // ------------------------------------------------------------------------
    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderScene->use();
    shaderScene->setMat4f("view",       camera.getViewMatrix());
    shaderScene->setMat4f("projection", projection);
    shaderScene->setVec3f("viewPos",    camera.position);

    // lighting
    // dirLight.render(shaderScene);
    pointLight.render(shaderScene);

    // modelFloor.draw(shaderScene, GL_TRIANGLES);
    modelCrate.draw(shaderScene, GL_TRIANGLES);
    modelCrate_2.draw(shaderScene, GL_TRIANGLES);

    // const auto time  = glfwGetTime();
    // light.position.x = glm::sin(time) * 10.0f;

    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  pool::ShaderPool::clear();
  pool::TexturePool::clear();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
  return 0;
}


