#ifndef OPENGL_OPENGLDEBUGGER_H
#define OPENGL_OPENGLDEBUGGER_H

#include <iostream>
#include <glad/glad.h>

namespace OBase
{
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
    };

} // OBase

#endif //OPENGL_OPENGLDEBUGGER_H
