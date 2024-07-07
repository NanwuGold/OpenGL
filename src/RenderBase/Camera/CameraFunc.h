#ifndef OPENGL_CAMERAFUNC_H
#define OPENGL_CAMERAFUNC_H

#include <RenderBase/Geometry/BoundingBox.h>
#include <glm/glm.hpp>

namespace OBase
{
    class CameraFunc
    {
    public:
        explicit CameraFunc(const BoundingBox &box, const glm::vec3 & front);

        glm::vec3 getTarget();

        glm::vec3 getPosition();

    private:
        BoundingBox m_fitBox;
        glm::vec3 m_front;
    };

} // OBase

#endif //OPENGL_CAMERAFUNC_H
