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
#include "mesh.hh"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#define WINDOW_WIDTH 720.0f
#define WINDOW_HEIGTH 720.0f

void loadVertices(const char* filename, std::vector<vertex_t>& vertices);

int main()
{ 
  Window window;
  window.create("OpenGL", WINDOW_WIDTH, WINDOW_HEIGTH);
  window.setPosition(200,200);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<vertex_t> vertices;
  loadVertices("res/CubeMeshTexture.obj", vertices);
  VertexBuffer vBuffer    { vertices.size(), vertices.data() };
  VertexArray vertexArray { vBuffer }; 

  // build and compile our shader program
  // ------------------------------------------------------------------------
  Shader cubeShader  { "shaders/cube.vertex.shader","shaders/cube.fragment.shader" };

  // create texture object
  // ------------------------------------------------------------------------
  Texture texContainerDiffuse  {"res/container.png", TextureType::TEX_DIFFUSE, true};
  Texture texContainerSpecular {"res/container_specular.png", TextureType::TEX_SPECULAR, true};

  // create mesh object
  // ------------------------------------------------------------------
  // ...

  
  glm::vec3   cubePos           = {0.0f, 0.0f, 0.0f};
  float       materialShininess = 64.0f;
  glm::vec3   lightColor        = {1.f, 1.f, 1.f}; // white color
  float       lightAmbient      = 0.05f; 
  float       lightDiffuse      = 0.5f;            // light intensity
  float       lightSpecular     = 1.f;
  glm::vec3   lightPos          = {0.0f,  2.0f, 5.0f};
  cubeShader.use();
  cubeShader.setInt("material.diffuse",  0);
  cubeShader.setInt("material.specular", 1);
  cubeShader.setFloat("material.shininess", materialShininess);
  cubeShader.setVec3("light.ambient",       lightColor * lightAmbient); 
  cubeShader.setVec3("light.diffuse",       lightColor * lightDiffuse);
  cubeShader.setVec3("light.specular",      lightColor * lightSpecular);
  cubeShader.setVec3("light.position",      lightPos);


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
    const glm::mat4 view       = camera.lookAt(cubePos);
    const glm::mat4 projection = glm::perspective(glm::radians(45.f), (WINDOW_WIDTH / WINDOW_HEIGTH), 0.1f, 100.0f);
    
    // draw the cube object
    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    cubeShader.use();
    cubeShader.setMat4("view", view);
    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("model", model);
    cubeShader.setVec3("viewPos", camera.position);

    Texture::activeTextUnit(0);
    texContainerDiffuse.bind();
    Texture::activeTextUnit(1);
    texContainerSpecular.bind();
    // vertexArray.disableAttribute(2);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());


    // Swapping buffers, processing events
    // ------------------------------------------------------------------------
    window.swapBuffersAndProcessEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  vertexArray.destroy();
  vBuffer.destroy();
  cubeShader.destroy();
  texContainerDiffuse.destroy();
  texContainerSpecular.destroy();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  window.terminate();
  return 0;
}


void loadVertices(const char* filename, std::vector<vertex_t>& vertices)
{
  std::ifstream f(filename);
  
  std::string line;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texcoords;
  
  int nVertices;
  while(std::getline(f, line))
  {
    if(line.find("vertices") != std::string::npos)
    {
      std::istringstream iss(line.substr(1));
      iss >> nVertices;
      break;
    }
  }

  vertices.reserve(nVertices);
  positions.reserve(nVertices);
  normals.reserve(nVertices);
  texcoords.reserve(nVertices);

  f.seekg(0);
  while(std::getline(f, line))
  {
    if(line[0] != 'v' && line[0] != 'f') 
      continue;

    // position
    if(line[0] == 'v' && line[1] == ' ')
    {
      std::istringstream iss(line.substr(1));
      glm::vec3 pos;
      iss >> pos.x;
      iss >> pos.y;
      iss >> pos.z;
      positions.push_back(pos);
    }
    // normal
    else if(line[0] == 'v' && line[1] == 'n')
    {
      std::istringstream iss(line.substr(2));
      glm::vec3 norm;
      iss >> norm.x;
      iss >> norm.y;
      iss >> norm.z;
      normals.push_back(norm);
    }
    // textcoord
    else if(line[0] == 'v' && line[1] == 't')
    {
      std::istringstream iss(line.substr(2));
      glm::vec2 tc;
      iss >> tc.x;
      iss >> tc.y;
      texcoords.push_back(tc);
    }
    // face
    else if(line[0] == 'f')
    {
    }
  }

  for(int i = 0; i < nVertices; i++)
  {
    std::array<float, vertex_t::VERTEX_COMPONENTS> values;
    values[0] = positions[i].x;
    values[1] = positions[i].y;
    values[2] = positions[i].z;
    values[3] = normals[i].x;
    values[4] = normals[i].y;
    values[5] = normals[i].z;
    values[6] = texcoords[i].x;
    values[7] = texcoords[i].y;
    // values = [x,y,z, x,y,z, u,v]
    vertices.push_back(vertex_t(values));
  }

  f.close();
}


