#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"

#include "pool/shader_pool.hh"
#include "pool/texture_pool.hh"

#include "lighting/directional_light.hh"
#include "lighting/point_light.hh"

#include "spdlog/spdlog.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main()
{ 
  Window window(vec2u(720, 720), vec2u(400,200), "OpenGL");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
  ImGui_ImplOpenGL3_Init("#version 130");

  pool::ShaderPool::initBuffer();
  pool::TexturePool::initBuffer();


  // load shaders
  // ------------------------------------------------------------------------
  auto shaderScene = pool::ShaderPool::loadShader("shaderMesh", "shaders/scene.vert","shaders/scene.frag");
  

  // create model objects
  // ------------------------------------------------------------------------
  Model modelCrate("assets/Crate/Crate.obj");
  // Model modelFloor("assets/Floor/Floor.obj");
  // modelFloor.setSize(vec3f(0.25f,0.25f,0.25f));
  // modelFloor.setPosition(vec3f(0.0,-1.0f,0.f));

  // create camera object
  // ------------------------------------------------------------------------
  vec3f target = modelCrate.position();

  Camera camera(modelCrate.position());
  camera.position.z = 10.f;
  camera.target = &target;

  // light object
  // ------------------------------------------------------------------------
  lighting::DirectionalLight dirLight("dirLight");
  //lighting::PointLight pointLight("pointLight");
  //pointLight.position.y = 10.f;

  
  
  // render loop
  // ------------------------------------------------------------------------
  while(window.loop())
  {
    // update delta time
    // ------------------------------------------------------------------------
    window.update();

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processInput(&window);
    //camera.processKeyboardInput(&window);
    //camera.processMouseScrollInput(&window);


    // Render 
    // ------------------------------------------------------------------------
    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4f projection = perspective(radians(camera.fov), (float)(window.width()/window.height()), 0.1f, 100.0f);
    shaderScene->use();
    shaderScene->setMat4f("view",       camera.getViewMatrix());
    shaderScene->setMat4f("projection", projection);
    shaderScene->setVec3f("viewPos",    camera.position);

    // lighting
    dirLight.render(shaderScene);
    // pointLight.render(shaderScene);
    
    modelCrate.draw(shaderScene, GL_TRIANGLES);
    // modelFloor.draw(shaderScene, GL_TRIANGLES);

#if 0
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if(ImGui::Begin("Directional light"))
    {
      ImGui::SliderFloat3("Direction", (float*) &dirLight.direction, -10.f, 10.f);
      ImGui::SliderFloat3("Color",     (float*) &dirLight.color,       0.f, 1.f);
      ImGui::SliderFloat("Ambient",    (float*) &dirLight.ambient,     0.f, 1.f);
      ImGui::SliderFloat("Diffuse",    (float*) &dirLight.diffuse,     0.f, 1.f);
      ImGui::SliderFloat("Specular",   (float*) &dirLight.specular,    0.f, 1.f);
    }
    ImGui::End();

    if(ImGui::Begin("Point light"))
    {
      ImGui::SliderFloat3("Position", (float*) &pointLight.position, -10.f, 10.f);
      ImGui::SliderFloat3("Color",    (float*) &pointLight.color,      0.f, 1.f);
      ImGui::SliderFloat("Ambient",   (float*) &pointLight.ambient,    0.f, 1.f);
      ImGui::SliderFloat("Diffuse",   (float*) &pointLight.diffuse,    0.f, 1.f);
      ImGui::SliderFloat("Specular",  (float*) &pointLight.specular,   0.f, 1.f);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();

    // window.msPerFrame();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  pool::ShaderPool::freeBuffer();
  pool::TexturePool::freeBuffer();


  // clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.destroy();
  return 0;
}


