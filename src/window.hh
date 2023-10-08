#ifndef WINDOW_HH
#define WINDOW_HH

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <map>

class Window
{
public:
  Window(const std::map<int,int>& hints);
  ~Window();
  
  GLFWwindow* create(std::string title, uint16_t width=720, uint16_t heigth=720);
  
  // set close flag TRUE
  void close();


  // return GLFWwindow* object
  GLFWwindow* get() const { return _window; }

  std::string getTitle() const { return _title; }

  uint16_t getWidth() const { return _width; }
  
  uint16_t getHeight() const { return _height; }

private:
  GLFWwindow* _window;
  
  std::string _title;
  uint16_t _width;
  uint16_t _height;

  bool _isInitialized;
};


#endif