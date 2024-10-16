#ifndef OPENGL_OBJECT_H
#define OPENGL_OBJECT_H

#include <string>

namespace OBase
{
    class Object
    {
    public:
        virtual std::string name() = 0;
        virtual void setName(const std::string_view & name) = 0;
    };
}

#endif //OPENGL_OBJECT_H
