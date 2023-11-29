#include "window.hh"

#include "spdlog/spdlog.h"

static void errorCallback(int error, const char* description)
{
  (void)error; // suppress werror 

  spdlog::error(description);
}


/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Window::Window(vec2u dim, vec2u pos, string title, bool fullscreen)
  : _width {dim.x}, _height{dim.y}
{
  glfwSetErrorCallback(errorCallback);

  if(!glfwInit())
  {
    spdlog::error("GLFW failed to initialize.");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  create(title, fullscreen);

  glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, this);
  
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // enable vsync
  // -----------------------------
  glfwSwapInterval(1); 

  // depth buffer
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // blending/stencil buffer
  // -----------------------------
  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // face culling
  // -----------------------------
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);  
  
  // antialiasing
  // -----------------------------
  // glEnable(GL_MULTISAMPLE); 
  // glfwWindowHint(GLFW_SAMPLES, 4);
 

  setPosition(pos);
  
  _prevFrame = 0; 
  _currFrame = 0;

  _lastTime  = glfwGetTime();
  _nbFrames  = 0;
}

void Window::destroy()
{
  if (_window)
    glfwDestroyWindow(_window);

  glfwTerminate();
}

void Window::processKeyboardInput()
{
  if(getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    close();
}

double Window::delta() const
{
  return _currFrame - _prevFrame;
}

void Window::update()
{
  _prevFrame = _currFrame;
  _currFrame = glfwGetTime();
}

void Window::msPerFrame()
{
  double currentTime = glfwGetTime();
  _nbFrames++;

  if ( currentTime - _lastTime >= 1.0 )
  {
    spdlog::info("{} ms/frame", 1000.0/double(_nbFrames));
    _nbFrames = 0;
    _lastTime += 1.0f;
  }
}

/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

void Window::create(string title, bool fullscreen)
{
  GLFWmonitor* monitor = nullptr;
  if(fullscreen)
    monitor = glfwGetPrimaryMonitor();

  _window = glfwCreateWindow(_width, _height, title.c_str(), monitor, NULL);
  if (!_window)
  {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
}