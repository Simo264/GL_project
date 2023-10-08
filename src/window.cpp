#include "window.hh"
#include "spdlog/spdlog.h"

#include<iterator> 
#include<algorithm>

Window::Window(const std::map<int,int>& hints) 
{
  _isInitialized = static_cast<bool>(glfwInit());
  if(!_isInitialized)
    throw std::runtime_error("GLFW failed to initialize.");

  for (const auto& [hint, value] : hints) 
    glfwWindowHint(hint, value);
}

Window::~Window()
{
  if (_isInitialized)
    glfwTerminate();

  if (_window)
    glfwDestroyWindow(m_Window);
}

GLFWwindow* Window::create(std::string title, uint16_t width, uint16_t heigth)
{
  _title = title; 
  _width = width; 
  _height = height;

  _window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!_window)
    throw std::runtime_error("Failed to create GLFW window");

  glfwMakeContextCurrent(_window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  return _window;
}

void Window::loop(std::function<void()>& renderCallback, std::function<void(double deltaTime)>& inputCallback)
{
  float deltaTime = 0.0f;	// time between current frame and last frame
  float lastFrame = 0.0f;
  while(!glfwWindowShouldClose(_window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // input
    inputCallback(deltaTime);

    // render
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderCallback();

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}

void Window::close()
{
  glfwSetWindowShouldClose(_window, GLFW_TRUE);
} 

void Window::release()
{
  auto windowMgr = getInstance();
  delete windowMgr;
}


uint16_t Window::getWidth() const
{
  return mWidth;
}

uint16_t Window::getHeigth() const
{
  return mHeigth;
}

