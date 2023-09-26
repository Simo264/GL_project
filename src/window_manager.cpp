#include "window_manager.hh"
#include "spdlog/spdlog.h"

WindowManager* WindowManager::getInstance()
{
  static WindowManager* windowManager = nullptr;
  if(!windowManager)
    windowManager = new WindowManager;
  
  return windowManager;
}

GLFWwindow* WindowManager::createWindow(const char* title, uint16_t w, uint16_t h)
{
  mWidth = w; 
  mHeigth = h;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow(mWidth, mHeigth, title, NULL, NULL);
  if (!mWindow)
  {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(mWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    spdlog::error("Failed to initialize GLAD");
    return nullptr;
  } 

  return mWindow;
}

void WindowManager::destroyWindow()
{
  glfwTerminate();
}

void WindowManager::loop(std::function<void(double delta)> inputCallback, 
  std::function<void(double delta)> renderCallback)
{

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  while(!glfwWindowShouldClose(mWindow))
  {
    const double delta = glfwGetTime();

    // input
    inputCallback(delta);

    // render
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderCallback(delta);

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }
}

void WindowManager::close()
{
  glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
} 

void WindowManager::release()
{
  auto windowMgr = getInstance();
  delete windowMgr;
}


uint16_t WindowManager::getWidth() const
{
  return mWidth;
}

uint16_t WindowManager::getHeigth() const
{
  return mHeigth;
}

