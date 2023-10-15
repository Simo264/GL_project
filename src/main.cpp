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

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
  ImGui_ImplOpenGL3_Init("#version 150");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);
  
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };
  VertexArray vertexArray { vBuffer }; 

  // build and compile our shader program
  // ------------------------------------
  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };
  shader.use();
  
  const glm::vec3 cubePos     = glm::vec3(0.0f,  0.0f,  0.0f);
  const glm::vec3 cubeAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
  const glm::vec3 cubeDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
  const glm::vec3 cubeSpecular= glm::vec3(0.5f, 0.5f, 0.5f);
  const float cubeShininess   = 32.0f;
  shader.setVec3("material.ambient",    cubeAmbient);
  shader.setVec3("material.diffuse",    cubeDiffuse);
  shader.setVec3("material.specular",   cubeSpecular);
  shader.setFloat("material.shininess", cubeShininess);
  
  glm::vec3 lightPos              = glm::vec3(2.0f,  2.0f, -5.0f);
  glm::vec3 lightColor            = glm::vec3(1.f, 1.f, 1.f);
  glm::vec3 lightAmbient          = glm::vec3(0.2f, 0.2f, 0.2f);
  glm::vec3 lightDiffuse          = glm::vec3(0.5f, 0.5f, 0.5f);
  const glm::vec3 lightSpecular   = glm::vec3(1.0f, 1.0f, 1.0f);
  shader.setVec3("lightPos",      lightPos);  
  shader.setVec3("light.ambient", lightAmbient);
  shader.setVec3("light.diffuse", lightDiffuse);
  shader.setVec3("light.specular",lightSpecular);

  // create texture object
  // ------------------------------------
  // Texture texture {"res/wall.jpg", true};

  // create camera object
  // ------------------------------------
  Camera camera { glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f) };


  bool show_demo_window = true;

  // render loop
  // -----------
  while(window.loop())
  {
    // update delta time
    // --------------------
    window.update();
    const double deltaTime = window.delta();

    // input
    // ------
    window.processKeyboardInput();
    camera.processKeyboardInput(window, deltaTime); 
    camera.processMouseMovement(window); 

    // Render 
    // ------
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      static float f = 0.0f;
      static int counter = 0;
      static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

      ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }
    ImGui::Render();
    
    window.clearColor(0.2f, 0.1f, 0.2f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    //texture.activeTextUnit(0);  
    //texture.bind();
    shader.use();
    vertexArray.bind();
    
    
    const glm::mat4 view       = camera.lookAround();
    const glm::mat4 projection = perspective(45.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model;
    
    // cube object
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    vertexArray.enableAttribute(1);
    shader.setMat4("model", model);
    shader.setBool("isLigthObject", false);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // light object
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.25,0.25,0.25));
    vertexArray.disableAttribute(1);
    shader.setMat4("model", model);
    shader.setBool("isLigthObject", true);
    glDrawArrays(GL_TRIANGLES, 0, 36);  

    const auto posX = sin(glfwGetTime()) * 5;
    const auto posZ = cos(glfwGetTime()) * 5;
    lightPos.x = posX;
    lightPos.z = posZ;
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);
    lightDiffuse = lightColor   * glm::vec3(0.5f); 
    lightAmbient = lightDiffuse * glm::vec3(0.2f);
    shader.setVec3("lightPos", lightPos); 
    shader.setVec3("lightColor", lightColor); 
    shader.setVec3("light.ambient", lightAmbient);
    shader.setVec3("light.diffuse", lightDiffuse);
    shader.setVec3("viewPos", camera.position); 
    
    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  shader.destroy();
  //texture.destroy();


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


