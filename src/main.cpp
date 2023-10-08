#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

#define VERTEX_COMPONENTS 5                // position=3 texture=2
#define VERTEX_LENGTH VERTEX_COMPONENTS*4  // sizeof(float)=4

void loadVertices(const char* filename, std::vector<float>& vertices);
void setVertexAttributes(VertexArray&, VertexBuffer&);
void setTextParams(Texture&);
void mouseInputCallback(GLFWwindow* window, double xpos, double ypos);

const std::map<int,int> WINDOW_HINTS = { 
  { GLFW_CONTEXT_VERSION_MAJOR, 3 }
  { GLFW_CONTEXT_VERSION_MINOR, 3 }
  { GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE }
};

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

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float currentFrame = 0.0f;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);


float lastX = 720.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

float yaw   = -90.0f;
float pitch = 0.0f;

int main()
{ 
  Window window { WINDOW_HINTS };

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(720, 720, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetCursorPosCallback(window, mouseInputCallback);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader zprogram
  // ------------------------------------
  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);

  VertexArray vertexArray { };
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };
  setVertexAttributes(vertexArray, vBuffer);

  Texture texture {"textures/wall.jpg", true};
  setTextParams(texture);

  shader.use();
  shader.setInt("texture1", 0);

  Camera camera { cameraPos,cameraFront,cameraUp };

  // key input callback 
  // ------------------------------------------------------------------------
  auto keyInputCallback = [&](double deltaTime){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.position += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.position -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
  };

  // render callback 
  // ------------------------------------------------------------------------
  auto renderCallback = [&](){
    texture.activeTextUnit(0);  
    texture.bind();

    shader.use();

    camera.front = cameraFront;
    camera.up = cameraUp;

    const glm::mat4 view = camera.lookAround();
    shader.setMat4("view", view);

    const glm::mat4 projection = glm::perspective(glm::radians(45.f), (720.0f/720.0f), 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    vertexArray.bind();
    for(size_t i = 0; i < CUBES.size(); i++)
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), CUBES[i]);
      model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  };



  // render loop
  // -----------
  while(!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // ------
    keyInputCallback(deltaTime);

    // Render 
    // ------
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderCallback();
    
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);  
    glfwPollEvents();         
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  shader.destroy();
  texture.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
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

void mouseInputCallback(GLFWwindow* window, double xposIn, double yposIn)
{
  (void)window;

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  yaw   += (xoffset * 0.1f);
  pitch += (yoffset * 0.1f);

  glm::vec3 front = { 
    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
    sin(glm::radians(pitch)),
    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
  };
  cameraFront = glm::normalize(front);

  auto right  = glm::normalize(glm::cross(cameraFront, {0.f,1.f,0.f}));
  cameraUp    = glm::normalize(glm::cross(right, cameraFront));
}



