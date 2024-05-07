#ifndef HAZEL_OPENGLVERTEXARRAY_H_
#define HAZEL_OPENGLVERTEXARRAY_H_

#include "OpenGLVertexArray.h"
#include <RenderBase/Render/VertexArray.h>

namespace OBase
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();

        ~OpenGLVertexArray() override;

        void Bind() const override;

        void UnBind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;

        void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

        const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const override;

        const Ref<IndexBuffer> &GetIndexBuffer() const override;

    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

    private:
        uint32_t m_RendererID;
    };
}

#endif // HAZEL_OPENGLVERTEXARRAY_H_
