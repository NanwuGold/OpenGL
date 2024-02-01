#include "FrameBuffer.h"
#include "RenderBase/OpenGL/OpenGLFrameBuffer.h"

OBase::Ref<FrameBuffer> FrameBuffer::Create()
{
    return OBase::CreateRef<OpenGLFrameBuffer>();
}
