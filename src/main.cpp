#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "window_manager.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"
#include "texture.hh"

#include <string>

void inputCallback()
{
  //spdlog::info("input...");
}

void renderCallback()
{
  glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{ 
  WindowManager* windowMgr = WindowManager::getInstance();
  windowMgr->createWindow("OpenGL");

  // build and compile our shader program
  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };


  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
    // position         // color        // texture
    0.5f, 0.5f,  0.0f,  1.0, 0.0, 0.0,  1.0, 1.0,   // top right
    0.5f, -0.5f, 0.0f,  0.0, 1.0, 0.0,  1.0, 0.0,   // bottom right
    -0.5f, -0.5f,0.0f,  0.0, 0.0, 1.0,  0.0, 0.0,   // bottom left
    -0.5f, 0.5f, 0.0f,  1.0, 0.0, 1.0,  0.0, 1.0,   // top left 
  };

  uint32_t indices[] = {  
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  };
  
  VertexArray vertexArray { };
  VertexBuffer vBuffer { sizeof(vertices),vertices };
  ElementBuffer eBuffer { sizeof(indices),indices };

  // 0 -> position
  vertexArray.vertexSpecification(0, 3, GL_FLOAT, 0); 
  vertexArray.bindBuffer(0, vBuffer.get(), 0, 8*sizeof(float));
  vertexArray.attribBinding(0, 0);
  vertexArray.enableAttribute(0);

  // 1 -> color
  vertexArray.vertexSpecification(1, 3, GL_FLOAT, 0); 
  vertexArray.bindBuffer(1, vBuffer.get(), 12, 8*sizeof(float));
  vertexArray.attribBinding(1, 1);
  vertexArray.enableAttribute(1);

  // 2 -> texture
  vertexArray.vertexSpecification(2, 2, GL_FLOAT, 0); 
  vertexArray.bindBuffer(2, vBuffer.get(), 24, 8*sizeof(float));
  vertexArray.attribBinding(2, 2);
  vertexArray.enableAttribute(2);


  // load and create a texture 
  // -------------------------
  Texture texture {"textures/wall.jpg"};
  texture.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  shader.use();
  shader.setInt("texture1", 0);

  windowMgr->loop(inputCallback, [texture, shader, vertexArray](){
    glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    texture.activeTextUnit(0);  
    texture.bind();

    shader.use();
    vertexArray.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  });

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  eBuffer.destroy();
  shader.destroy();
  texture.destroy();

  windowMgr->destroyWindow();
  windowMgr->release();
  return 0;
}

