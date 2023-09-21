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
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow(w, h, title, NULL, NULL);
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

void WindowManager::loop(std::function<void()> inputCallback, std::function<void()> renderCallback)
{
  while(!glfwWindowShouldClose(mWindow))
  {
    // input
    if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);
    inputCallback();

    // render
    renderCallback();

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }
}

void WindowManager::release()
{
  auto windowMgr = getInstance();
  delete windowMgr;
}
