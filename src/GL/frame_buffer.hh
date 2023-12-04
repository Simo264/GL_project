#ifndef FRAME_BUFFER_HH
#define FRAME_BUFFER_HH

#include "../core.hh"
#include "../mesh2d.hh"

namespace GL
{
  class FrameBuffer
  {
  public:
    FrameBuffer(vec2i framebufferSize);
    ~FrameBuffer() = default;

    FrameBuffer(const FrameBuffer&) = delete;            // delete copy constructor
    FrameBuffer& operator=(const FrameBuffer&) = delete; // delete assign op

    void bindFB(uint32_t target) { glBindFramebuffer(target, _frameBufferID); }
    void bindIFB(uint32_t target){ glBindFramebuffer(target, _intermediateFrameBufferID); }
    void blit() { glBlitFramebuffer(0, 0, _fbSize.x, _fbSize.y, 0, 0, _fbSize.x, _fbSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST); }

    void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    
    void destroy();
    void draw();

  private:
    vec2i _fbSize;

    uint32_t _frameBufferID;
    uint32_t _textureColorBufferMultiSampledID;
    uint32_t _renderBufferID;
    uint32_t _intermediateFrameBufferID;
    uint32_t _textureScreenID;

    Mesh2D   _screenImage;

    bool checkStatus();
  };
}
#endif


