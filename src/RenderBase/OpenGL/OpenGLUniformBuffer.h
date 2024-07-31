#ifndef OPENGL_OPENGLUNIFORMBUFFER_H
#define OPENGL_OPENGLUNIFORMBUFFER_H

#include <RenderBase/Render/UniformBuffer.h>

namespace OBase
{
    class OpenGLUniformBuffer final : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(const UniformLayout& layout, uint8_t bindPoint);
        ~OpenGLUniformBuffer() override;

        OpenGLUniformBuffer() = delete;
        OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer&) = delete;
        OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&&) = delete;
        OpenGLUniformBuffer(OpenGLUniformBuffer&&) = delete;
        OpenGLUniformBuffer(OpenGLUniformBuffer&) = delete;

        void Bind() override;
        void UnBind() override;
        void LinkBindingPoint() override;

        UniformLayout& GetLayout() override;
        void UpdateElementData(const std::string & name, const void * data) override;

        uint8_t BindPoint() override;
    protected:
        void invaild();
        void UpdateData(uint32_t offset, uint32_t size, const void *data);

    private:
        unsigned int m_RenderID;
        uint32_t m_Size;       ///< 数据的存储大小
        uint8_t m_BindPoint;   ///< 当前uniform的绑定点 同一次draw call中只能绑定到一个bind point上
        UniformLayout m_UniformLayout;
    };

} // OBase

#endif //OPENGL_OPENGLUNIFORMBUFFER_H
