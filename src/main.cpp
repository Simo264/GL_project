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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

#define VERTEX_COMPONENTS 5   // position=3 texture=2
#define VERTEX_LENGTH VERTEX_COMPONENTS*4  // 4=sizeof(float)

void loadVertices(const char* filename, std::vector<float>& vertices);

int main()
{ 
  WindowManager* windowMgr = WindowManager::getInstance();
  windowMgr->createWindow("OpenGL");

  // build and compile our shader program
  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);

  VertexArray vertexArray { };
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };

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


  // load and create a texture 
  // -------------------------
  Texture texture {"textures/wall.jpg", true};
  texture.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  shader.use();
  shader.setInt("texture1", 0);

  /* input callback */
  auto inputCallback = [&](double delta){
    if(glfwGetKey(windowMgr->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
      windowMgr->close();

    delta = delta + 0; // avoids werror
  };
  
  /* render callback */
  auto renderCallback = [&](double delta){
    texture.activeTextUnit(0);  
    texture.bind();

    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(
      model, 
      glm::radians(static_cast<float>(delta * 100 * glm::radians(50.0f))), 
      glm::vec3(0.5f, 1.0f, 0.0f));

    const glm::mat4 view = glm::translate(
      glm::mat4(1.0f), 
      glm::vec3(0.0f, 0.0f, -3.0f));
    
    const glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), // fov 45.0 
      static_cast<float>(windowMgr->getWidth()/windowMgr->getHeigth()),
      0.1f, 
      100.0f);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    vertexArray.bind();
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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

