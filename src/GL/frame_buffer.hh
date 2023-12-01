#ifndef FRAME_BUFFER_HH
#define FRAME_BUFFER_HH

#include "../core.hh"
#include "../mesh2d.hh"

namespace GL
{
  class FrameBuffer
  {
  public:
    FrameBuffer();
    ~FrameBuffer() = default;

    FrameBuffer(const FrameBuffer&) = delete;            // delete copy constructor
    FrameBuffer& operator=(const FrameBuffer&) = delete; // delete assign op

    void bind(uint32_t target = GL_FRAMEBUFFER)   { glBindFramebuffer(target, _buffer); }
    void unbind(uint32_t target = GL_FRAMEBUFFER) { glBindFramebuffer(target, 0); }
    void destroy();

    // the framebuffer checked is that bound to target, 
    // which must be GL_DRAW_FRAMEBUFFER | GL_READ_FRAMEBUFFER | GL_FRAMEBUFFER
    bool checkStatus(uint32_t target = GL_FRAMEBUFFER) const;
  
    uint32_t texture() const { return _texture; }

    void draw();

  private:
    uint32_t _buffer;
    uint32_t _texture;
    uint32_t _renderBuffer;

    Mesh2D _screenImage;
  };
}
#endif


