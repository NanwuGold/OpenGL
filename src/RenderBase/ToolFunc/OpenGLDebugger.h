#ifndef OPENGL_OPENGLDEBUGGER_H
#define OPENGL_OPENGLDEBUGGER_H

#include <functional>
#include <iostream>
#include <glad/glad.h>

namespace OBase
{

    static std::function<void(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)> outFunc;

    class OpenGLDebugger
    {
    public:
        static void APIENTRY DebugCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

        static void EnableDebug();

        static void AddCallBack(const std::function<void(GLenum,
            GLenum,
            GLuint,
            GLenum,
            GLsizei,
            const GLchar*,
            const void*)> & func)
        {
            outFunc = func;
               
        }

        static void DisableDebug();

    private:
        static bool m_EnableDebug;

    };

} // OBase

#endif //OPENGL_OPENGLDEBUGGER_H
