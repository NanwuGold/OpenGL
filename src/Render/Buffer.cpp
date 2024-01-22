#include <hzpch.h>
#include "Buffer.h"

#include <Hazel/Renderer/Renderer.h>
#include <Hazel/Core/Base.h>
#include <Hazel/Core/Log.h>
#include <Platform/OpenGL/OpenGLBuffer.h>

namespace Hazel
{
    Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
    {
        HZ_PROFILE_FUNCTION()

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }
        HZ_CORE_ASSERT(false, "UnKnown RendererAPI!")
        return nullptr;
    }

    Ref <VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        HZ_PROFILE_FUNCTION()

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(size);
        }
        HZ_CORE_ASSERT(false, "UnKnown RendererAPI!")
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {
        HZ_PROFILE_FUNCTION()

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RenderAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>(indices, count);
        }
        HZ_CORE_ASSERT(false, "UnKnown RendererAPI!")
        return nullptr;
    }

}
