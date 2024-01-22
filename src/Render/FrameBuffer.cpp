#include "FrameBuffer.h"
#include "OpenGL/OpenGLFrameBuffer.h"

FXAA::Ref<FrameBuffer> FrameBuffer::Create()
{
    return FXAA::CreateRef<OpenGLFrameBuffer>();
}
