#ifndef OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H
#define OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H

#include <unordered_map>
#include <RenderBase/Render/FrameBuffer.h>

namespace OBase
{
    class OpenGLFrameBuffer final : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer();
        ~OpenGLFrameBuffer() override;

        OpenGLFrameBuffer(const OpenGLFrameBuffer&) = default;
        OpenGLFrameBuffer(OpenGLFrameBuffer&&) = default;
        
        OpenGLFrameBuffer& operator=(const OpenGLFrameBuffer&) = default;
        OpenGLFrameBuffer& operator=(OpenGLFrameBuffer&&) = default;

        void Create(std::vector<Ref<Texture>> colorAttachments, Ref<Texture> depth) override;

        void Bind() override;

        void Resize(int w, int h) override;

        void UnBind() override;

        uint32_t RenderID() override;

        std::shared_ptr<Texture> GetAttachment(const FramebufferAttachment& index) override;

    protected:
        void Invalidate();

    private:
        uint32_t m_RendererID;
        std::vector<Ref<Texture>> m_colorAttachments;
        std::unordered_map<FramebufferAttachment, Ref<Texture>> m_Attachments;
        Ref<Texture> m_depthAttachment;
    };
}


#endif //OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H