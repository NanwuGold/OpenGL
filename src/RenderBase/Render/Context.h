#ifndef OPENGL_RENDERBASE_CONTEXT_H
#define OPENGL_RENDERBASE_CONTEXT_H

namespace OBase
{
	class Context
	{
	public:
		Context() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}




#endif // !OPENGL_RENDERBASE_CONTEXT_H
