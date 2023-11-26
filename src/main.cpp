#include "core.hh"

#include "window.hh"
#include "shader.hh"
#include "camera.hh"
#include "model.hh"
#include "stencil.hh"

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

  // auto normalMap = pool::TexturePool::loadTexture("assets/Cube/normal.png");


#if 0
  // simple grass
  // ------------------------------------------------------------------------
  auto texTransparentWindow = pool::TexturePool::loadTexture("res/blending_transparent_window.png");
  auto texGrass             = pool::TexturePool::loadTexture("res/grass.png");

  vector<Vertex> vertices = {
    Vertex{vec3f(0.5f,  0.5f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(1.0f, 1.0f)}, // top right
    Vertex{vec3f(0.5f, -0.5f,  0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(1.0f, 0.0f)}, // bottom right
    Vertex{vec3f(-0.5f, -0.5f, 0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(0.0f, 0.0f)}, // bottom left
    Vertex{vec3f(-0.5f,  0.5f, 0.0f),  vec3f(0.0f, 0.0f, 0.0f),  vec2f(0.0f, 1.0f)}, // top left 
  };
  vector<uint32_t> indices = {0,1,3, 1,2,3};
  vector<Texture*> textures = {texTransparentWindow, texGrass}; (void) textures;
  VertexBuffer  vertexBuffer(vertices.size(), vertices.data());
  ElementBuffer elementBuffer(indices.size(), indices.data());
  VertexArray   vertexArray(&vertexBuffer);
#endif

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
  camera.sensitivity = 128.0f;

  // create model objects
  // ------------------------------------------------------------------------
  Model modelFloor("assets/Floor/Floor.obj");
  modelFloor.setSize(vec3f(0.25f,0.25f,0.25f));
  modelFloor.setPosition(vec3f(0.0,-1.0f,0.f));

  Model modelCrate("assets/Crate/Crate.obj");
  modelCrate.setPosition(vec3f(10.0f, 0.0125f, 0.0f));
  
  Model modelCube("assets/Cube/Cube.obj");
  modelCube.setPosition(vec3f(10.0f, 0.0125f, 5.0f));

  // light object
  // ------------------------------------------------------------------------
  lighting::DirectionalLight dirLight("dirLight");    (void)dirLight;
#if 0
  lighting::PointLight pointLight("pointLight");      (void)pointLight;
  lighting::SpotLight spotLight("spotLight");         (void)spotLight;
#endif
  

#if 0
  // stencil object
  // ------------------------------------------------------------------------
  Stencil stencil(shaderOutline); (void) stencil;
#endif

  // render loop
  // ------------------------------------------------------------------------
  while(window.loop())
  {
    window.update(); // update delta time

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processInput(&window);

    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // View/Projection matrices
    // ------------------------------------------------------------------------
    const mat4f projection = glm::perspective(
      glm::radians(camera.fov), (float)(window.width()/window.height()), 0.1f, 100.0f);
    const mat4f view = camera.getViewMatrix();

#if 0
    shaderOutline->use();
    shaderOutline->setMat4f("view",       view);
    shaderOutline->setMat4f("projection", projection);
#endif
  
    shaderScene->use();
    shaderScene->setMat4f("view",       view);
    shaderScene->setMat4f("projection", projection);
    shaderScene->setVec3f("viewPos",    camera.position);

    // Render lights
    // ------------------------------------------------------------------------
    dirLight.render(shaderScene);
#if 0
    pointLight.render(shaderScene);
    spotLight.render(shaderScene);
#endif

    // Render models
    // ------------------------------------------------------------------------
    modelFloor.draw(shaderScene);
    modelCrate.draw(shaderScene);
    modelCube.draw(shaderScene);
#if 0    
    stencil.drawOutline(&modelCrate2, shaderScene, vec3f(1.0f, 0.0f, 0.0f), 0.5f);
#endif

#if 0
    auto model = mat4f(1.0f);
    model = glm::translate(model, vec3f(8.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3f(0.0f, 1.0f, 0.0f));

    shaderBlending->use();
    shaderBlending->setMat4f("view", view);
    shaderBlending->setMat4f("projection", projection);
    shaderBlending->setMat4f("model", model);
    shaderBlending->setInt("texture1", 0);

    vertexArray.bind();
    elementBuffer.bind();
    
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
    
    Texture::activeTextUnit(0);
    texGrass->bind();
    //texTransparentWindow->bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    vertexArray.unbind();
    elementBuffer.unbind();
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

    // window.msPerFrame();
  }

  pool::ShaderPool::freeBuffer();
  pool::TexturePool::freeBuffer();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  window.destroy();

  return 0;
}


