#include "FrameBuffer.h"
#include "RenderBase/OpenGL/OpenGLFrameBuffer.h"

namespace OBase
{
    Ref<FrameBuffer> FrameBuffer::Create()
    {
        return OBase::CreateRef<OpenGLFrameBuffer>();
    }
}