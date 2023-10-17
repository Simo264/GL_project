#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "vertex.hh"
#include "window.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"
#include "texture.hh"
#include "camera.hh"
#include "mesh"

#include <string>
#include <fstream>
#include <sstream>

#define WINDOW_WIDTH 720.0f
#define WINDOW_HEIGTH 720.0f

void loadVertices(const char* filename, std::vector<float>& vertices);
const glm::mat4 perspective(float fov);

int main()
{ 
  Window window;
  window.create("OpenGL", WINDOW_WIDTH, WINDOW_HEIGTH);
  window.setPosition(200,200);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
  ImGui_ImplOpenGL3_Init("#version 150");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);
  
  VertexBuffer vBuffer    { vertices.size(), vertices.data() };
  VertexArray vertexArray { vBuffer }; 

  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader cubeShader  { "shaders/cube.vertex.shader","shaders/cube.fragment.shader" };
  Shader lightShader { "shaders/light.vertex.shader","shaders/light.fragment.shader" };
  
  
  glm::vec3   cubePos           = {0.0f, 0.0f, 0.0f};
  float       materialShininess = 64.0f;
  glm::vec3   lightColor        = {1.f, 1.f, 1.f}; // white color
  float       lightAmbient      = 0.05f; 
  float       lightDiffuse      = 0.5f;            // light intensity
  float       lightSpecular     = 1.f;
  cubeShader.use();
  cubeShader.setInt("material.diffuse",  0);
  cubeShader.setInt("material.specular", 1);
  cubeShader.setFloat("material.shininess", materialShininess);
  cubeShader.setVec3("light.ambient",       lightColor * lightAmbient); 
  cubeShader.setVec3("light.diffuse",       lightColor * lightDiffuse);
  cubeShader.setVec3("light.specular",      lightColor * lightSpecular);

  glm::vec3 lightObjectPos = {0.0f,  2.0f, 5.0f};

  // create texture object
  // ------------------------------------------------------------------------
  Texture texContainerDiffuse  {"res/container.png", TextureType::TEX_DIFFUSE, true};
  Texture texContainerSpecular {"res/container_specular.png", TextureType::TEX_SPECULAR, true};


  // create camera object
  // ------------------------------------------------------------------------
  Camera camera { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f) };
   
  // render loop
  // ------------------------------------------------------------------------
  glm::mat4 model;
  while(window.loop())
  {
    // update delta time
    // ------------------------------------------------------------------------
    window.update();
    const double deltaTime = window.delta();

    // input
    // ------------------------------------------------------------------------
    window.processKeyboardInput();
    camera.processKeyboardInput(window, deltaTime); 
    camera.processMouseMovement(window); 

    // Render 
    // ------------------------------------------------------------------------
    window.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // view/projection transformations
    const glm::mat4 view       = camera.lookAround();
    const glm::mat4 projection = perspective(45.0f);
    
    // draw the cube object
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    cubeShader.use();
    cubeShader.setMat4("view", view);
    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("model", model);
    cubeShader.setVec3("light.position", lightObjectPos);
    cubeShader.setVec3("viewPos", camera.position);
    
    Texture::activeTextUnit(0);
    texContainerDiffuse.bind();
    
    Texture::activeTextUnit(1);
    texContainerSpecular.bind();

    vertexArray.enableAttribute(1);
    vertexArray.enableAttribute(2);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // draw the lamp object
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightObjectPos);
    model = glm::scale(model, glm::vec3(0.2f)); 
    lightShader.use();
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("model", model);
    vertexArray.disableAttribute(1);
    vertexArray.disableAttribute(2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // render Imgui
    // ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplGlfw_NewFrame();
    // ImGui::NewFrame();
    // ImGui::Begin("Light object");                          
    // ImGui::SliderFloat3("light pos", (float*) &lightObjectPos, -5.f, 5.f);
    // ImGui::SliderFloat("shininess", &materialShininess, 0, 256);
    // ImGui::SliderFloat3("light color",   (float*) &lightColor, 0.0f, 1.f);
    // ImGui::SliderFloat("light ambient",  &lightAmbient,   0.0f, 1.f);
    // ImGui::SliderFloat("light diffuse",  &lightDiffuse,   0.0f, 1.f);
    // ImGui::SliderFloat("light specular", &lightSpecular,  0.0f, 1.f);
    // ImGui::End();
    // ImGui::Render();
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  lightShader.destroy();
  cubeShader.destroy();
  texContainerDiffuse.destroy();
  texContainerSpecular.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  window.terminate();
  return 0;
}



void loadVertices(const char* filename, std::vector<float>& vertices)
{
  std::ifstream f(filename);
  
  const int lines = std::count(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(), '\n');
  vertices.reserve(lines * vertex_t::VERTEX_COMPONENTS);

  f.seekg(0,std::ios::beg);

  std::string line;
  std::getline(f, line); // skip first line

  while(std::getline(f, line))
  {
    std::istringstream iss(line);
    std::string sVal;
    while (iss >> sVal) 
    {
      float fValue = std::stof(sVal);
      vertices.push_back(fValue);
    }
  }

  f.close();
}

const glm::mat4 perspective(float fov)
{
  return glm::perspective(
    glm::radians(fov), 
    static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGTH), 
    0.1f,
    100.0f); 
}


