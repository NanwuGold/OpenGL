#ifndef OPENGL_OPENGLUNIFORMBUFFER_H
#define OPENGL_OPENGLUNIFORMBUFFER_H

#include <RenderBase/Render/Buffer.h>
#include <glad/glad.h>

namespace OBase
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(GLsizeiptr size, GLuint bindPoint);
        ~OpenGLUniformBuffer() override  = default;

        OpenGLUniformBuffer() = delete;

        OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer&) = delete;
        OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&&) = delete;

        OpenGLUniformBuffer(OpenGLUniformBuffer&&) = delete;
        OpenGLUniformBuffer(OpenGLUniformBuffer&) = delete;

        void Bind() override;

        void UnBind() override;

    };

} // OBase

#endif //OPENGL_OPENGLUNIFORMBUFFER_H
