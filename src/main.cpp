#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
  
  const glm::vec3 cubeObjectPos     = glm::vec3(0.0f,  0.0f,  0.0f);
  glm::vec3 lightObjectPos    = glm::vec3(2.0f,  0.0f, -5.0f);
  const glm::vec3 cubeObjectColor   = glm::vec3(1.0f, 0.5f, 0.31f);
  const glm::vec3 lightObjectColor  = glm::vec3(1.0f, 1.0f, 1.0f);
  shader.setVec3("objectColor", cubeObjectColor);
  shader.setVec3("lightColor",  lightObjectColor);
  shader.setVec3("lightPos", lightObjectPos);  

  // create texture object
  // ------------------------------------
  // Texture texture {"res/wall.jpg", true};

  // create camera object
  // ------------------------------------
  Camera camera { glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f) };




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
    window.clearColor(0.5f, 0.5f, 0.8f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    //texture.activeTextUnit(0);  
    //texture.bind();
    shader.use();
    vertexArray.bind();
    
    
    const glm::mat4 view       = camera.lookAround();
    const glm::mat4 projection = perspective(45.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model;
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubeObjectPos);
    vertexArray.enableAttribute(1);
    shader.setMat4("model", model);
    shader.setBool("ligthObject", false);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, lightObjectPos);
    vertexArray.disableAttribute(1);
    shader.setMat4("model", model);
    shader.setBool("ligthObject", true);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightObjectPos.y = sin(glfwGetTime()) * 5;
    shader.setVec3("lightPos", lightObjectPos); 
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


