#ifndef OPENGL_GEOMETRYOBJECT_H
#define OPENGL_GEOMETRYOBJECT_H

#include "Object.h"
#include <vector>
#include <glm/vec3.hpp>

namespace OBase
{
    class GeometryObject : public Object
    {
    public:
        virtual void render() = 0;
        virtual void setNormal(std::vector<glm::vec3> & normals) = 0;
        virtual void setVertex(std::vector<glm::vec3> & vertexes) = 0;
        virtual void setIndex(std::vector<unsigned int> & indexes) = 0;
        virtual void create() = 0;
    };
}

#endif //OPENGL_GEOMETRYOBJECT_H
