#include <hzpch.h>

#include "VertexArray.h"

#include <Hazel/Core/Log.h>

#include <Hazel/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Hazel
{
    Ref<VertexArray> Hazel::VertexArray::Create()
    {
        HZ_PROFILE_FUNCTION()

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RenderAPI::None is currently not supported!")
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
        }

        HZ_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }


}
