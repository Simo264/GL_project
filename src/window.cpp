#include "window.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

void Window::create(vec2u dim, vec2u pos, const char* title)
{
  _width = dim.x;
  _height= dim.y;
  _window = glfwCreateWindow(_width, _height, title, NULL, NULL);
  glfwMakeContextCurrent(_window);
  setPosition(pos);
}

void Window::destroy()
{
  if (_window)
    glfwDestroyWindow(_window);
}

void Window::processKeyboardInput()
{
  if(getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    close();
}

