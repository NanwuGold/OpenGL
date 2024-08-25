#include "OpenGLDebugger.h"

namespace OBase
{

    bool OpenGLDebugger::m_EnableDebug = false;

    void OpenGLDebugger::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar *message, const void *userParam)
    {
        std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
                  << " type = " << type
                  << ", severity = " << severity
                  << ", message = " << message << std::endl;


        if(outFunc)
        {
            outFunc(source, type, id, severity, length, message, userParam);
        }

    }

    void OpenGLDebugger::EnableDebug()
    {
#if _DEBUG
        if(!m_EnableDebug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
            m_EnableDebug = true;
        }
#endif
    }

    void OpenGLDebugger::DisableDebug()
    {
#if _DEBUG
        if (!m_EnableDebug)
        {
            return;
        }

        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(nullptr, nullptr);
        m_EnableDebug = false;
#endif
    }
} // OBase