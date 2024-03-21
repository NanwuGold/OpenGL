#ifndef OPENGL_RENDERBASE_OPENGLTEXTURE_H
#define OPENGL_RENDERBASE_OPENGLTEXTURE_H

#include "RenderBase/Render/Texture.h"

class OpenGLTexture final : public Texture
{
public:

    ~OpenGLTexture() override;

    OpenGLTexture(int w, int h, GLenum format);

    OpenGLTexture(OpenGLTexture&&) = delete;
    OpenGLTexture(OpenGLTexture&) = delete;

    OpenGLTexture& operator=(OpenGLTexture&&) = delete;
    OpenGLTexture& operator=(OpenGLTexture&) = delete;

    void Bind() override;

    void resize(int w, int h) override;

    void Invalidate() override;

    unsigned int RenderID() override;

    void Create() override;

    GLenum format() override;

    bool multiSampleFlag() override;


private:
    unsigned int m_TextureID;
    unsigned int m_Width;
    unsigned int m_Height;
    GLenum m_Format;
};


class OpenGLMultiSampleTexture: public Texture
{
public:
    OpenGLMultiSampleTexture(int w, int h, GLenum format, MultiSample sample = MultiSample::X4);

    OpenGLMultiSampleTexture(OpenGLMultiSampleTexture&&) = delete;
    OpenGLMultiSampleTexture(OpenGLMultiSampleTexture&) = delete;

    OpenGLMultiSampleTexture& operator=(OpenGLMultiSampleTexture&&) = delete;
    OpenGLMultiSampleTexture& operator=(OpenGLMultiSampleTexture&) = delete;

    ~OpenGLMultiSampleTexture() override;

    void Bind() override;
    void resize(int w, int h) override;
    void Create() override;
    void Invalidate() override;
    GLenum format() override;
    unsigned RenderID() override;

    bool multiSampleFlag() override;

private:
    unsigned int m_TextureID;
    unsigned int m_Width;
    unsigned int m_Height;
    GLenum m_Format;
    int m_Sample{4};
};


#endif //OPENGL_RENDERBASE_OPENGLTEXTURE_H
