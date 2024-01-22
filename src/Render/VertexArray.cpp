#include "VertexArray.h"
#include "Render/OpenGL/OpenGLVertexArray.h"
#include "pointer_ptr.hpp"

namespace FXAA
{
    Ref<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
}
