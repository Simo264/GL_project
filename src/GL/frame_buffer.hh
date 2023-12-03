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

    void bind();   
    void unbind(); 
    void destroy();
    void draw();

  private:
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


