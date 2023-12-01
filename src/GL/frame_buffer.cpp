#include "frame_buffer.hh"
#include "spdlog/spdlog.h"

namespace GL
{
  FrameBuffer::FrameBuffer()
  {
    glGenFramebuffers(1, &_frameBufferID);
    
    bind();
    
    // generate texture and attach it to framebuffer object
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 720, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureID, 0);

    // generate render buffer and attach it to framebuffer object
    glGenRenderbuffers(1, &_renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferID); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 720, 720);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBufferID);

    if(!checkStatus(GL_FRAMEBUFFER))
      spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    unbind();
  }

  bool FrameBuffer::checkStatus(uint32_t target) const
  {
    return glCheckNamedFramebufferStatus(_frameBufferID, target) == GL_FRAMEBUFFER_COMPLETE;
  }

  void FrameBuffer::draw()
  {
    glBindTexture(GL_TEXTURE_2D, _textureID);
    _screenImage.draw();
  }

  void FrameBuffer::destroy()
  {
    glDeleteTextures(1, &_textureID);
    glDeleteRenderbuffers(1, &_renderBufferID);
    glDeleteFramebuffers(1, &_frameBufferID);
  }
}