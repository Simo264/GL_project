#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "window_manager.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"
#include "texture.hh"
#include "camera.hh"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

#define VERTEX_COMPONENTS 5                // position=3 texture=2
#define VERTEX_LENGTH VERTEX_COMPONENTS*4  // sizeof(float)=4

std::vector<glm::vec3> CUBES = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};
glm::vec3& CUBE_TARGET = CUBES[0];

void loadVertices(const char* filename, std::vector<float>& vertices);
void setVertexAttributes(VertexArray&, VertexBuffer&);
void setTextParams(Texture&);

int main()
{ 
  WindowManager* windowMgr = WindowManager::getInstance();
  auto window = windowMgr->createWindow("OpenGL");

  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };

  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);

  VertexArray vertexArray { };
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };
  setVertexAttributes(vertexArray, vBuffer);

  Texture texture {"textures/wall.jpg", true};
  setTextParams(texture);

  shader.use();
  shader.setInt("texture1", 0);

  const glm::mat4 projection = glm::perspective(
    glm::radians(45.f), // FOV=45.0
    static_cast<float>(windowMgr->getWidth()/windowMgr->getHeigth()),
    0.1f, 100.0f);
  
  // if we want the camera to move backwards, we move along the positive z-axis
  const glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 5.0f);
  const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

  Camera camera { cameraPos,cameraFront };
  
  // input callback 
  // ------------------------------------------------------------------------
  auto inputCallback = [&](double deltaTime){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      windowMgr->close();
    
    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.position += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.position -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.position -= glm::normalize(glm::cross(cameraFront, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.position += glm::normalize(glm::cross(cameraFront, camera.up)) * cameraSpeed;
  };
 
  // render callback 
  // ------------------------------------------------------------------------
  auto renderCallback = [&](double deltaTime){
    deltaTime = deltaTime + 0;

    texture.activeTextUnit(0);  
    texture.bind();

    shader.use();

    vertexArray.bind();
    for(size_t i = 0; i < CUBES.size(); i++)
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), CUBES[i]);
      model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    const glm::mat4 view = camera.lookAt(CUBE_TARGET);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
  };
  

  windowMgr->loop(inputCallback, renderCallback);


  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  shader.destroy();
  texture.destroy();

  windowMgr->destroyWindow();
  windowMgr->release();
  return 0;
}



void loadVertices(const char* filename, std::vector<float>& vertices)
{
  std::ifstream f(filename);
  
  const int lines = std::count(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(), '\n');
  vertices.reserve(lines * VERTEX_COMPONENTS);

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

void setVertexAttributes(VertexArray& vertexArray, VertexBuffer& vBuffer)
{
  // 0 -> position
  vertexArray.vertexSpecification(0, 3, GL_FLOAT, 0); 
  vertexArray.bindBuffer(0, vBuffer.get(), 0, VERTEX_LENGTH);
  vertexArray.attribBinding(0, 0);
  vertexArray.enableAttribute(0);
  // 1 -> texture
  vertexArray.vertexSpecification(1, 2, GL_FLOAT, 0); 
  vertexArray.bindBuffer(1, vBuffer.get(), 12, VERTEX_LENGTH);
  vertexArray.attribBinding(1, 1);
  vertexArray.enableAttribute(1);
}

void setTextParams(Texture& texture)
{
  texture.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

