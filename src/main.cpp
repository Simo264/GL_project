#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"
#include "stencil.hh"
#include "surface2d.hh"

#include "pool/shader_pool.hh"
#include "pool/texture_pool.hh"

#include "lighting/directional_light.hh"
#include "lighting/point_light.hh"
#include "lighting/spot_light.hh"

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

  auto texTransparentWindow = pool::TexturePool::loadTexture("res/blending_transparent_window.png");
  auto texGrass             = pool::TexturePool::loadTexture("res/grass.png");
  (void) texTransparentWindow;
  (void) texGrass;


  // load shaders
  // ------------------------------------------------------------------------
  auto shaderScene   = pool::ShaderPool::loadShader("shaderMesh", "shaders/scene.vert","shaders/scene.frag");
  auto shaderOutline = pool::ShaderPool::loadShader("shaderOutline", "shaders/outline.vert","shaders/outline.frag");
  auto shaderBlending= pool::ShaderPool::loadShader("shaderBlending", "shaders/blending.vert","shaders/blending.frag");
  (void) shaderScene;
  (void) shaderOutline;
  (void) shaderBlending;

  // create camera object
  // ------------------------------------------------------------------------
  Camera camera;



  // create model objects
  // ------------------------------------------------------------------------
  Model modelFloor("assets/Floor/Floor.obj");
  modelFloor.scale(vec3f(0.125f, 1.0f, 0.125f));
  modelFloor.translate(vec3f(0.0,-1.0f,0.f));

  Model modelCrate("assets/Crate/Crate.obj");
  modelCrate.translate(vec3f(10.0f, 0.0125f, 0.0f));
  
  Model modelCube("assets/Cube/Cube.obj");
  modelCube.translate(vec3f(10.0f, 0.0125f, 5.0f));

  // light object
  // ------------------------------------------------------------------------
  lighting::DirectionalLight dirLight("dirLight");    (void)dirLight;
  lighting::PointLight pointLight("pointLight");      (void)pointLight;
  lighting::SpotLight spotLight("spotLight");         (void)spotLight;
  
#if 0
  // stencil object
  // ------------------------------------------------------------------------
  Stencil stencil(shaderOutline); (void) stencil;
#endif

  // grass
  // ------------------------------------------------------------------------
  Surface2D surface;
  surface.diffuse = texGrass;
  surface.translate(vec3f(0.0f, 1.0f, 0.0f));

  // render loop
  // ------------------------------------------------------------------------
  const mat4f projection = glm::perspective(glm::radians(camera.fov), (float)(window.width()/window.height()), 0.1f, 100.0f);
  while(window.loop())
  {
    window.update(); // update delta time
    window.msPerFrame();

    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processInput(&window);

    
    // View/Projection matrices
    // ------------------------------------------------------------------------
    const mat4f view = camera.getViewMatrix();
  
    shaderScene->use();
    shaderScene->setMat4f("view",       view);
    shaderScene->setMat4f("projection", projection);
    shaderScene->setVec3f("viewPos",    camera.position);
    shaderScene->setInt("material.diffuse", 0);
    shaderScene->setInt("material.normal", 1);
    shaderScene->setInt("material.specular", 2);
    shaderScene->setFloat("material.shininess", 32.0f);

    // Render lights
    // ------------------------------------------------------------------------
    dirLight.render(shaderScene);
    //pointLight.render(shaderScene);
    //spotLight.render(shaderScene);

    // Render models
    // ------------------------------------------------------------------------
    modelFloor.draw(shaderScene);
    modelCrate.draw(shaderScene);
    modelCube.draw(shaderScene);

#if 0
    // render grass
    // ------------------------------------------------------------------------
    glDisable(GL_CULL_FACE);
    shaderBlending->use();
    shaderBlending->setMat4f("view",       view);
    shaderBlending->setMat4f("projection", projection);
    shaderBlending->setMat4f("model",      surface.model());
    surface.draw(shaderBlending, GL_TRIANGLES);
    glEnable(GL_CULL_FACE);
#endif

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

#if 0
    if(ImGui::Begin("Directional light"))
    {
      ImGui::SliderFloat3("Direction", (float*) &dirLight.direction, -10.f, 10.f);
      ImGui::SliderFloat3("Color",     (float*) &dirLight.color,       0.f, 1.f);
      ImGui::SliderFloat("Ambient",    (float*) &dirLight.ambient,     0.f, 1.f);
      ImGui::SliderFloat("Diffuse",    (float*) &dirLight.diffuse,     0.f, 1.f);
      ImGui::SliderFloat("Specular",   (float*) &dirLight.specular,    0.f, 1.f);
    }
    ImGui::End();
#endif
#if 0
    if(ImGui::Begin("Spot light"))
    {
      ImGui::SliderFloat3("Position", (float*) &spotLight.position,  -10.f, 10.f);
      ImGui::SliderFloat3("Direction",(float*) &spotLight.direction, -10.f, 10.f);
      ImGui::SliderFloat3("Color",    (float*) &spotLight.color,      0.f, 1.f);
      ImGui::SliderFloat("Ambient",   (float*) &spotLight.ambient,    0.f, 1.f);
      ImGui::SliderFloat("Diffuse",   (float*) &spotLight.diffuse,    0.f, 1.f);
      ImGui::SliderFloat("Specular",  (float*) &spotLight.specular,   0.f, 1.f);
      ImGui::SliderFloat("Radius",    (float*) &spotLight.cutOff,     0.f, 90.f);
    }
    ImGui::End();
#endif
#if 0
    if(ImGui::Begin("Point light"))
    {
      ImGui::SliderFloat3("Position", (float*) &pointLight.position, -10.f, 10.f);
      ImGui::SliderFloat3("Color",    (float*) &pointLight.color,      0.f, 1.f);
      ImGui::SliderFloat("Ambient",   (float*) &pointLight.ambient,    0.f, 1.f);
      ImGui::SliderFloat("Diffuse",   (float*) &pointLight.diffuse,    0.f, 1.f);
      ImGui::SliderFloat("Specular",  (float*) &pointLight.specular,   0.f, 1.f);
    }
    ImGui::End();
#endif

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window.swapBuffersAndProcessEvents();
  }

  modelFloor.destroy();
  modelCrate.destroy();
  modelCube.destroy();
  surface.destroy();

  pool::ShaderPool::freeBuffer();
  pool::TexturePool::freeBuffer();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  window.destroy();

  return 0;
}


