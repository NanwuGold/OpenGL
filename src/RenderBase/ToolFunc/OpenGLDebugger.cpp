#include "OpenGLDebugger.h"

namespace OBase
{
    void OpenGLDebugger::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar *message, const void *userParam)
    {
        std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
                  << " type = " << type
                  << ", severity = " << severity
                  << ", message = " << message << std::endl;
    }

    void OpenGLDebugger::EnableDebug()
    {
#if _DEBUG
        static std::once_flag sCallOnce;
        std::call_once(sCallOnce, []()
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
        });
#endif
    }
} // OBase