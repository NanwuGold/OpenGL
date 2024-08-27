#include "OpenGLTexture.h"

#include <iostream>


namespace 
{
    std::pair<GLenum, GLenum> GetClearFormatAndType(const GLenum internalFormat)
    {
        switch (internalFormat)
        {
            // 8-bit formats
        case GL_R8:
            return { GL_RED, GL_UNSIGNED_BYTE };
        case GL_RG8:
            return { GL_RG, GL_UNSIGNED_BYTE };
        case GL_RGB8:
            return { GL_RGB, GL_UNSIGNED_BYTE };
        case GL_RGBA8:
            return { GL_RGBA, GL_UNSIGNED_BYTE };

            // 16-bit formats
        case GL_R16:
            return { GL_RED, GL_UNSIGNED_SHORT };
        case GL_RG16:
            return { GL_RG, GL_UNSIGNED_SHORT };
        case GL_RGB16:
            return { GL_RGB, GL_UNSIGNED_SHORT };
        case GL_RGBA16:
            return { GL_RGBA, GL_UNSIGNED_SHORT };

            // 32-bit formats
        case GL_R32F:
            return { GL_RED, GL_FLOAT };
        case GL_RG32F:
            return { GL_RG, GL_FLOAT };
        case GL_RGB32F:
            return { GL_RGB, GL_FLOAT };
        case GL_RGBA32F:
            return { GL_RGBA, GL_FLOAT };

        case GL_R32I:
            return { GL_RED_INTEGER, GL_INT };
        case GL_RG32I:
            return { GL_RG_INTEGER, GL_INT };
        case GL_RGB32I:
            return { GL_RGB_INTEGER, GL_INT };
        case GL_RGBA32I:
            return { GL_RGBA_INTEGER, GL_INT };

        case GL_R32UI:
            return { GL_RED_INTEGER, GL_UNSIGNED_INT };
        case GL_RG32UI:
            return { GL_RG_INTEGER, GL_UNSIGNED_INT };
        case GL_RGB32UI:
            return { GL_RGB_INTEGER, GL_UNSIGNED_INT };
        case GL_RGBA32UI:
            return { GL_RGBA_INTEGER, GL_UNSIGNED_INT };
        default:
            throw std::runtime_error("Unsupported internal format for clear operation");
        }
    }

}


OpenGLTexture::OpenGLTexture(int w, int h, GLenum internalFormat)
    : MidTexture()
    , m_TextureID(0)
    , m_Width(w)
    , m_Height(h)
    , m_InternalFormat(internalFormat)
{
    
}

void OpenGLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D,m_TextureID);
}

void OpenGLTexture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::BindImage(const uint8_t bindingPoint)
{
    m_ImageBindingPoint = bindingPoint;
    glBindImageTexture(m_ImageBindingPoint, m_TextureID, 0,GL_FALSE,0,GL_READ_WRITE, m_InternalFormat);
}

void OpenGLTexture::UnBindImage()
{
    glBindImageTexture(m_ImageBindingPoint, 0, 0, GL_FALSE, 0, GL_READ_WRITE, m_InternalFormat);
}

void OpenGLTexture::resize(const int w, const int h)
{
    m_Width = w;
    m_Height = h;

    Invalidate();
}

void OpenGLTexture::Invalidate()
{
    if(m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
    }
    glCreateTextures(GL_TEXTURE_2D,1,&m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTextureStorage2D(m_TextureID, 1 , m_InternalFormat, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

unsigned int OpenGLTexture::RenderID()
{
    return m_TextureID;
}

void OpenGLTexture::Create()
{
    Invalidate();
}

GLenum OpenGLTexture::InternalFormat() const
{
    return m_InternalFormat;
}

bool OpenGLTexture::multiSampleFlag()
{
    return false;
}

unsigned OpenGLTexture::GetWidth()
{
    return m_Width;
}

unsigned OpenGLTexture::GetHeight()
{
    return m_Height;
}

void OpenGLTexture::Clear(const GLint level, const void* data)
{
    if(glClearTexImage)
    {
        const auto & [clearFormat, clearDataType] = GetClearFormatAndType(InternalFormat());
        glClearTexImage(m_TextureID, level, clearFormat, clearDataType, data);
    }
    else
    {
        /// do nothing!!!
    }
}

OpenGLTexture::~OpenGLTexture()
{
    if (m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
    }
}


OpenGLMultiSampleTexture::OpenGLMultiSampleTexture(int w, int h, GLenum format, MultiSample sample)
    :ConMultiSampleTexture()
    , m_TextureID(0)
    , m_Width(w)
    , m_Height(h)
    , m_InternalFormat(format)
    , m_Sample(static_cast<int>(sample))
{
}

OpenGLMultiSampleTexture::~OpenGLMultiSampleTexture()
{
    if (m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
    }
}

void OpenGLMultiSampleTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
}

void OpenGLMultiSampleTexture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void OpenGLMultiSampleTexture::resize(const int w, const int h)
{
    m_Width = w;
    m_Height = h;
    Invalidate();
}

void OpenGLMultiSampleTexture::Create()
{
    Invalidate();
}

void OpenGLMultiSampleTexture::Invalidate()
{
    if(m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
    glTextureStorage2DMultisample(m_TextureID, m_Sample, m_InternalFormat, static_cast<GLsizei>(m_Width),
    static_cast<GLsizei>(m_Height), GL_FALSE);
}

GLenum OpenGLMultiSampleTexture::InternalFormat() const
{
    return m_InternalFormat;
}

unsigned OpenGLMultiSampleTexture::RenderID()
{
    return m_TextureID;
}

unsigned OpenGLMultiSampleTexture::GetWidth()
{
    return m_Width;
}

unsigned OpenGLMultiSampleTexture::GetHeight()
{
    return m_Height;
}

bool OpenGLMultiSampleTexture::multiSampleFlag()
{
    return true;
}

void OpenGLMultiSampleTexture::Clear(const GLint level, const void* data)
{
    if(glClearTexImage)
    {
        auto [clearDataFormat, clearDataType] = GetClearFormatAndType(m_InternalFormat);
        glClearTexImage(m_TextureID, level, clearDataFormat, clearDataType, data);
    }
    else
    {
        /// do no thing!!!
    }
}


