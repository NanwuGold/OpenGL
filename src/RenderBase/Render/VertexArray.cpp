#include "VertexArray.h"
#include "RenderBase/OpenGL/OpenGLVertexArray.h"
#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{
    Ref<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
}
