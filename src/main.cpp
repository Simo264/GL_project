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
  window.setPosition(200,200);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);
  
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };
  VertexArray vertexArray { vBuffer }; 

  // build and compile our shader program
  // ------------------------------------
  Shader cubeShader  { "shaders/cube.vertex.shader","shaders/cube.fragment.shader" };
  Shader lightShader { "shaders/light.vertex.shader","shaders/light.fragment.shader" };
  
  
  const glm::vec3 cubePos           = {0.0f, 0.0f, 0.0f};
  const int       materialDiffuse   = 0;
  const glm::vec3 materialSpecular  = {0.5f, 0.5f, 0.5f};
  const float     materialShininess = 64.0f;
  const glm::vec3 lightAmbient      = {0.2f, 0.2f, 0.2f};
  const glm::vec3 lightDiffuse      = {0.5f, 0.5f, 0.5f};
  const glm::vec3 lightSpecular     = {1.0f, 1.0f, 1.0f};
  cubeShader.use();
  cubeShader.setInt("material.diffuse",     materialDiffuse);
  cubeShader.setVec3("material.specular",   materialSpecular);
  cubeShader.setFloat("material.shininess", materialShininess);
  cubeShader.setVec3("light.ambient",       lightAmbient); 
  cubeShader.setVec3("light.diffuse",       lightDiffuse);
  cubeShader.setVec3("light.specular",      lightSpecular);

  
  glm::vec3 lightObjectPos = {2.0f,  2.0f, -5.0f};


  // create texture object
  // ------------------------------------
  Texture texContainer {"res/container.png", true};

  // create camera object
  // ------------------------------------
  Camera camera { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f) };

   
  // render loop
  // -----------
  glm::mat4 model;
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
    window.clearColor(0.2f, 0.1f, 0.2f, 1.0f);
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
    texContainer.bind();
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
  texContainer.destroy();


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


