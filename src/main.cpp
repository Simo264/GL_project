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

#define VERTEX_COMPONENTS 5                // position=3 texture=2
#define VERTEX_LENGTH VERTEX_COMPONENTS*4  // sizeof(float)=4

void loadVertices(const char* filename, std::vector<float>& vertices);
void setVertexAttributes(VertexArray&, VertexBuffer&);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(const Window& window, Camera& camera, float deltaTime);

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

// camera

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);


float lastX = 720.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

float yaw   = -90.0f;
float pitch = 0.0f;

int main()
{ 
  Window window;
  window.create("OpenGL", 720, 720);
  window.setCursorPosCallback(cursorPositionCallback);
 
  // build and compile our shader program
  // ------------------------------------
  Shader shader { "shaders/vertex.shader","shaders/fragment.shader" };

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<float> vertices;
  loadVertices("res/vertices.txt", vertices);
  VertexBuffer vBuffer { static_cast<uint32_t>(vertices.size()*sizeof(float)), vertices.data() };

  VertexArray vertexArray { vBuffer };
  
  setVertexAttributes(vertexArray, vBuffer);

  Texture texture {"textures/wall.jpg", true};
  setTextParams(texture);

  shader.use();
  shader.setInt("texture1", 0);


  const glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 5.0f);

  Camera camera { cameraPos,cameraFront,cameraUp };


  // render loop
  // -----------
  float deltaTime = 0.0f;	
  float lastFrame = 0.0f;
  float currentFrame = 0.0f;
  while(window.loop())
  {
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // ------
    processInput(window, camera, deltaTime);

    // Render 
    // ------
    window.clearColor(0.5f, 0.5f, 0.8f, 1.0f);
    window.clearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    
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
    
    // Swapping buffers, processing events
    // ------
    window.swapBuffersAndProcessEvents();  
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  shader.destroy();
  texture.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
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
  
}

void cursorPositionCallback(GLFWwindow* window, double xposIn, double yposIn)
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

void processInput(const Window& window, Camera& camera, float deltaTime)
{
  const float cameraSpeed = 2.5f * deltaTime;

  if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
    camera.position += cameraSpeed * camera.front;
  
  if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
    camera.position -= cameraSpeed * camera.front;
  
  if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
    camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
  
  if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
    camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
}

