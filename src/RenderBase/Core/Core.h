#ifndef OPENGL_RENDERBASE_CORE_H
#define OPENGL_RENDERBASE_CORE_H

#define OBASE_DEBUGBREAK() __debugbreak()

#define OBASE_ASSERT(x,...) \
{                           \
    if(!x)                  \
    {                       \
        std::cout<<__VA_ARGS__<< std::endl; \
        OBASE_DEBUGBREAK(); \
    } \
}

#define OBASE_INFO(x) \
{                     \
    std::cout<< x << std::endl;  \
}

#endif