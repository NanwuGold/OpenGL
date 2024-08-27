#ifndef OPENGL_RENDERBASE_CONTEXT_H
#define OPENGL_RENDERBASE_CONTEXT_H

namespace OBase
{
    class Context
    {
    public:
        virtual ~Context() = default;
        Context() = default;

        Context(const Context&) = delete;
        Context(Context&&) = delete;

        Context& operator=(const Context&) = delete;
        Context& operator=(Context&&) = delete;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}




#endif // !OPENGL_RENDERBASE_CONTEXT_H
