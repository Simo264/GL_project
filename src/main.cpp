#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "window_manager.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"

#include <string>

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";
  
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";


void inputCallback()
{
  spdlog::info("input...");
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
  Shader shader { vertexShaderSource,fragmentShaderSource };


  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
    0.5f,  0.5f,   // top right
    0.5f, -0.5f,   // bottom right
    -0.5f, -0.5f,  // bottom left
    -0.5f,  0.5f,  // top left 
  };
  uint32_t indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  };

  
  VertexArray vertexArray { };
  VertexBuffer vBuffer { sizeof(vertices),vertices };
  ElementBuffer eBuffer { sizeof(indices),indices };

  vertexArray.vertexSpecification(0, 2, GL_FLOAT, 0);
  vertexArray.enableAttribute(0);

  windowMgr->loop(inputCallback, [shader, vertexArray](){
    glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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

  windowMgr->destroyWindow();
  windowMgr->release();
  return 0;
}

