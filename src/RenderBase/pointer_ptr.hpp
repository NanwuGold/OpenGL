#ifndef OPENGL_POINER_PTR
#define OPENGL_POINER_PTR

#include <memory>
#include <functional>

namespace OBase
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args &&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    constexpr Ref<T> CreateRef(T * ptr)
    {
        return Ref<T>(ptr);
    }
}

#endif