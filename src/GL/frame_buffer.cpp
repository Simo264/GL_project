#include "frame_buffer.hh"
#include "spdlog/spdlog.h"

namespace GL
{

  /* -----------------------------------------------------
  *          PUBLIC METHODS
  * -----------------------------------------------------
  */

  FrameBuffer::FrameBuffer(vec2i framebufferSize) : _fbSize{framebufferSize}
  {
    // configure MSAA framebuffer
    // --------------------------
    const uint32_t samples = 4;

    glGenFramebuffers(1, &_frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);

    // create a multisampled color attachment texture
    glGenTextures(1, &_textureColorBufferMultiSampledID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _textureColorBufferMultiSampledID);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, _fbSize.x, _fbSize.y, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _textureColorBufferMultiSampledID, 0);
    
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    glGenRenderbuffers(1, &_renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, _fbSize.x, _fbSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBufferID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    glGenFramebuffers(1, &_intermediateFrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFrameBufferID);
    
    // create a color attachment texture
    glGenTextures(1, &_textureScreenID);
    glBindTexture(GL_TEXTURE_2D, _textureScreenID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _fbSize.x, _fbSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureScreenID, 0);	

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      spdlog::error("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::draw()
  {
    glBindTexture(GL_TEXTURE_2D, _textureScreenID);
    _screenImage.draw();
  }

  void FrameBuffer::destroy()
  {
    glDeleteFramebuffers(1, &_frameBufferID);
    glDeleteTextures(1, &_textureColorBufferMultiSampledID);
    glDeleteRenderbuffers(1, &_renderBufferID);

    glDeleteFramebuffers(1, &_intermediateFrameBufferID);
    glDeleteTextures(1, &_textureScreenID);
  }

  /* -----------------------------------------------------
  *          PRIVATE METHODS
  * -----------------------------------------------------
  */

  bool FrameBuffer::checkStatus()
  {
    return glCheckNamedFramebufferStatus(_frameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  }
}