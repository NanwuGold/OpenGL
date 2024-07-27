#ifndef HAZEL_OPENGLBUFFER_H
#define HAZEL_OPENGLBUFFER_H

#include <RenderBase/Render/Buffer.h>

namespace OBase
{
    class OpenGLVertexBuffer final: public VertexBuffer
    {
    public:
        OpenGLVertexBuffer() = delete;

        explicit OpenGLVertexBuffer(uint32_t size);
        explicit OpenGLVertexBuffer(float *vertices, uint32_t size);

        OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer(OpenGLVertexBuffer &&) = delete;

        OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&&) = delete;

        ~OpenGLVertexBuffer() override;

        void Bind() const override;

        void UnBind() const override;

        void SetLayout(const BufferLayout &layout) override;

        [[nodiscard]] const BufferLayout &GetLayout() const override;

        void SetDate(void *data, uint32_t size) override;

    private:
        uint32_t m_RendererID{};
        BufferLayout m_Layout{};

    };

    class OpenGLIndexBuffer final: public IndexBuffer
    {
    public:

        OpenGLIndexBuffer(OpenGLIndexBuffer&) = default;
        OpenGLIndexBuffer(OpenGLIndexBuffer&&) = default;

        OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = default;
        OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&&) = default;

        OpenGLIndexBuffer(const uint32_t *indices, uint32_t count);

        ~OpenGLIndexBuffer() override;

        void Bind() const override;

        void UnBind() const override;

        [[nodiscard]] uint32_t GetCount() override;

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}
#endif // HAZEL_OPENGLBUFFER_H
