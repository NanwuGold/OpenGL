#include "OpenGLTexture.h"

#include <iostream>


namespace 
{
    GLenum GetClearTexImageType(const GLenum format)
    {
        switch (format) {
        case GL_RGBA8:
        case GL_RGB8:
        case GL_RED:
        case GL_RED_INTEGER:
        case GL_RG:
            return GL_UNSIGNED_BYTE;
        case GL_RGBA16F:
        case GL_RGB16F:
        case GL_R16F:
            return GL_HALF_FLOAT;
        case GL_RGBA32F:
        case GL_RGB32F:
        case GL_R32F:
            // return GL_FLOAT;
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            return GL_FLOAT;
        case GL_DEPTH_COMPONENT16:
            return GL_UNSIGNED_SHORT;
        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
            throw std::invalid_argument("Depth-stencil formats are not supported for clear type.");
        default:
            throw std::invalid_argument("Unsupported internal format.");
        }
    }
}


OpenGLTexture::OpenGLTexture(int w, int h, GLenum format)
    : m_TextureID(0)
    , m_Width(w)
    , m_Height(h)
    , m_Format(format)
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

void OpenGLTexture::resize(int w, int h)
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

    glTextureStorage2D(m_TextureID, 1 , m_Format, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
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

GLenum OpenGLTexture::format()
{
    return m_Format;
}

bool OpenGLTexture::multiSampleFlag()
{
    return false;
}

void OpenGLTexture::Clear(const GLint level, const void* data)
{
    if(glClearTexImage)
    {
        glClearTexImage(m_TextureID, level, m_Format, GetClearTexImageType(format()), data);
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
    : Texture()
    , m_TextureID(0)
    , m_Width(w)
    , m_Height(h)
    , m_Format(format)
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
    glTextureStorage2DMultisample(m_TextureID, m_Sample, m_Format, static_cast<GLsizei>(m_Width),
    static_cast<GLsizei>(m_Height), GL_FALSE);
}

GLenum OpenGLMultiSampleTexture::format()
{
    return m_Format;
}

unsigned OpenGLMultiSampleTexture::RenderID()
{
    return m_TextureID;
}

bool OpenGLMultiSampleTexture::multiSampleFlag()
{
    return true;
}

void OpenGLMultiSampleTexture::Clear(const GLint level, const void* data)
{
    if(glClearTexImage)
    {
        glClearTexImage(m_TextureID, level, m_Format, GetClearTexImageType(format()), data);
    }
}


