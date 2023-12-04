#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"
#include "stencil.hh"
#include "mesh2d.hh"
#include "skybox.hh"

#include "GL/frame_buffer.hh"

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
  // init libraries and create GLFW window
  // ---------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4); // enable 4x MSAA on GLFW framebuffer

  
  Window window;
  window.create(vec2u(720,720), vec2u(400,400), "Opengl");
  
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1); // v-sync on

  // antialising
  glEnable(GL_MULTISAMPLE);
  // depth buffer
  glEnable(GL_DEPTH_TEST);
  // blending/stencil buffer
  glDisable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // face culling
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);  
  // ---------------------------------------
  
  
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

  // blending textures
  // ------------------------------------------------------------------------
  auto textWindow = pool::TexturePool::loadTexture("res/blending_transparent_window.png");
  auto textGrass  = pool::TexturePool::loadTexture("res/grass.png");
  (void) textWindow; (void) textGrass;


  // load shaders
  // ------------------------------------------------------------------------
  auto shaderScene = pool::ShaderPool::loadShader("shaderScene", "shaders/scene.vert","shaders/scene.frag");
  shaderScene->use();
  shaderScene->setInt("material.diffuse", 0);
  shaderScene->setInt("material.normal", 1);
  shaderScene->setInt("material.specular", 2);
  shaderScene->setFloat("material.shininess", 32.0f);
  
  auto shaderFB = pool::ShaderPool::loadShader("shaderFB", "shaders/frame_buffer.vert","shaders/frame_buffer.frag");
  shaderFB->use();
  shaderFB->setInt("screenTexture", 0);
  
  auto shaderOutline = pool::ShaderPool::loadShader("shaderOutline", "shaders/outline.vert","shaders/outline.frag");
  shaderOutline->use();
  shaderOutline->setVec3f("outlineColor", vec3f(0.25f, 0.50f, 0.75f));
  
  auto shaderBlend = pool::ShaderPool::loadShader("shaderBlending", "shaders/blending.vert","shaders/blending.frag");
  shaderBlend->use();
  shaderBlend->setInt("material.diffuse", 0);
  
  auto shaderSky = pool::ShaderPool::loadShader("shaderSkybox", "shaders/skybox.vert","shaders/skybox.frag");
  shaderSky->use();
  shaderSky->setInt("skybox", 0);

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

  array<string, 6> skyboxImages = {
    "res/Skybox/right.jpg",
    "res/Skybox/left.jpg",
    "res/Skybox/top.jpg",
    "res/Skybox/bottom.jpg",
    "res/Skybox/front.jpg",
    "res/Skybox/back.jpg",
  };
  SkyBox skybox(skyboxImages);

  // light object
  // ------------------------------------------------------------------------
  lighting::DirectionalLight dirLight("dirLight");
  lighting::PointLight pointLight("pointLight");      (void)pointLight;
  lighting::SpotLight spotLight("spotLight");         (void)spotLight;

#if 0 // create custom MSAA framebuffer
  vec2i fbSize;
  window.getFramebufferSize(fbSize);
  GL::FrameBuffer frameBuffer(fbSize); (void) frameBuffer;
#endif

  const double fpsLimit = 1.0 / 60.0;
  double lastUpdateTime = 0;  // number of seconds since the last loop
  double lastFrameTime  = 0;   // number of seconds since the last frame

  // render loop
  // ------------------------------------------------------------------------
  while(window.loop())
  {
    // per-frame time logic
    // --------------------
    double now = glfwGetTime();
    double deltaTime = now - lastUpdateTime;
    
    // input
    // ------------------------------------------------------------------------
    glfwPollEvents();
    window.processKeyboardInput();
    camera.processInput(window, deltaTime);
    mat4f view = camera.getViewMatrix();
    mat4f projection = glm::perspective(glm::radians(camera.fov), (float)(window.width()/window.height()), 0.1f, 100.0f);

    // render
    // ------------------------------------------------------------------------
    if ((now - lastFrameTime) >= fpsLimit)
    {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               // values for the color buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers to preset values

      // draw scene here
      // ----------------------------------
      glEnable(GL_DEPTH_TEST);
      shaderScene->use();
      shaderScene->setMat4f("view",       view);
      shaderScene->setMat4f("projection", projection);
      shaderScene->setVec3f("viewPos",    camera.position);
      dirLight.render(shaderScene);
      modelFloor.draw(shaderScene, GL_TRIANGLES);
      glEnable(GL_CULL_FACE);
      modelCrate.draw(shaderScene, GL_TRIANGLES);
      modelCube.draw(shaderScene, GL_TRIANGLES);
      glDisable(GL_CULL_FACE);
      
      glDepthFunc(GL_LEQUAL);
      shaderSky->use();
      shaderSky->setMat4f("view",       mat4f(mat3f(view)));
      shaderSky->setMat4f("projection", projection);
      skybox.draw();
      glDepthFunc(GL_LESS);
      // ----------------------------------

      // only set lastFrameTime when you actually draw something
      lastFrameTime = now;

      window.swapBuffers();
    }
    
    // set lastUpdateTime every iteration
    lastUpdateTime = now;
    
    
  #if 0 // use custom framebuffer
    frameBuffer.bindFB(GL_FRAMEBUFFER); 
  #endif

  #if 0 // render on custom framebuffer
    // antialiasing frame buffer slows down performance
    frameBuffer.bindFB(GL_READ_FRAMEBUFFER);
    frameBuffer.bindIFB(GL_DRAW_FRAMEBUFFER);
    frameBuffer.blit();

    frameBuffer.unbind();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    shaderFB->use();
    frameBuffer.draw();
  #endif

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

  #if 0
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  #endif

    
  }

  modelFloor.destroy();
  modelCrate.destroy();
  modelCube.destroy();
  skybox.destroy();
  
  pool::ShaderPool::freeBuffer();
  pool::TexturePool::freeBuffer();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
#if 0 // destroy custom framebuffer
  frameBuffer.destroy();
#endif

  window.destroy();
  glfwTerminate();
  return 0;
}


