#ifndef OPENGL_RENDERBASE_OPENGLTEXTURE_H
#define OPENGL_RENDERBASE_OPENGLTEXTURE_H

#include <RenderBase/Render/Texture.h>

class MidTexture : public Texture
{
public:
    MidTexture() = default;

    MidTexture(MidTexture&&) = delete;
    MidTexture(MidTexture&) = delete;
    MidTexture& operator=(MidTexture&&) = delete;
    MidTexture& operator=(MidTexture&) = delete;

    ~MidTexture() override = default;
    void Bind() override {}
    void UnBind() override {}
    void BindImage(uint8_t bindingPoint) override {}
    void UnBindImage() override {}
    void resize(int w, int h) override {}
    void Create() override {}
    unsigned GetWidth() override { return 0; }
    unsigned GetHeight() override { return 0; }
    GLenum InternalFormat() const override { return GL_RGBA; }
    unsigned RenderID() override { return 0; }
    bool multiSampleFlag() override { return true; }
    void Clear(GLint level, const void* data) override {}
};


class OpenGLTexture final : public MidTexture
{
public:
    OpenGLTexture(int w, int h, GLenum format);
    ~OpenGLTexture() override;

    OpenGLTexture(OpenGLTexture&&) = delete;
    OpenGLTexture(OpenGLTexture&) = delete;

    OpenGLTexture& operator=(OpenGLTexture&&) = delete;
    OpenGLTexture& operator=(OpenGLTexture&) = delete;

    void Bind() override;

    void UnBind() override;

    void BindImage(uint8_t bindingPoint) override;

    void UnBindImage() override;

    void resize(int w, int h) override;

    unsigned int RenderID() override;

    void Create() override;

    GLenum InternalFormat() const override;

    bool multiSampleFlag() override;

    unsigned GetWidth() override;

    unsigned GetHeight() override;

    void Clear(GLint level, const void* data) override;

protected:
    void Invalidate();

private:
    unsigned int m_TextureID;   ///< 纹理ID
    unsigned int m_Width;       ///< 纹理宽度
    unsigned int m_Height;      ///< 纹理高度
    GLenum m_InternalFormat;    ///< 纹理数据显存中的存储方式
    GLenum m_DataFormat{GL_RGBA};   ///< 纹理数据格式
    GLenum m_DataType{GL_UNSIGNED_BYTE}; ///< 纹理数据类型
    uint8_t m_ImageBindingPoint{0};
};


class ConMultiSampleTexture : public Texture
{
public:
    ConMultiSampleTexture() = default;

    ConMultiSampleTexture(ConMultiSampleTexture&&) = delete;
    ConMultiSampleTexture(ConMultiSampleTexture&) = delete;
    ConMultiSampleTexture& operator=(ConMultiSampleTexture&&) = delete;
    ConMultiSampleTexture& operator=(ConMultiSampleTexture&) = delete;

    ~ConMultiSampleTexture() override = default;
    void Bind() override {}
    void UnBind() override {}
    void BindImage(uint8_t bindingPoint) override{}
    void UnBindImage() override{}
    void resize(int w, int h) override{}
    void Create() override{}
    unsigned GetWidth() override { return 0; }
    unsigned GetHeight() override { return 0; }
    GLenum InternalFormat() const override { return GL_RGBA; }
    unsigned RenderID() override { return 0; }
    bool multiSampleFlag() override { return true; }
    void Clear(GLint level, const void* data) override {}
};


class OpenGLMultiSampleTexture: public ConMultiSampleTexture
{
public:
    OpenGLMultiSampleTexture(int w, int h, GLenum format, MultiSample sample = MultiSample::X4);

    OpenGLMultiSampleTexture(OpenGLMultiSampleTexture&&) = delete;
    OpenGLMultiSampleTexture(OpenGLMultiSampleTexture&) = delete;

    OpenGLMultiSampleTexture& operator=(OpenGLMultiSampleTexture&&) = delete;
    OpenGLMultiSampleTexture& operator=(OpenGLMultiSampleTexture&) = delete;

    ~OpenGLMultiSampleTexture() override;

    void Bind() override;
    void UnBind() override;
    void resize(int w, int h) override;
    void Create() override;
    GLenum InternalFormat() const override;
    unsigned RenderID() override;
    unsigned GetWidth() override;
    unsigned GetHeight() override;

    bool multiSampleFlag() override;

    void Clear(GLint level, const void* data) override;

protected:
    void Invalidate();

private:
    unsigned int m_TextureID;
    unsigned int m_Width;
    unsigned int m_Height;
    GLenum m_InternalFormat;   ///< 纹理数据在显存中的存储格式
    int m_Sample{4};
};


#endif //OPENGL_RENDERBASE_OPENGLTEXTURE_H
