#ifndef OPENGL_CAMERAFUNC_H
#define OPENGL_CAMERAFUNC_H

#include <RenderBase/Geometry/BoundingBox.h>
#include <glm/glm.hpp>

namespace OBase
{
    class CameraFunc
    {
    public:
        explicit CameraFunc(const BoundingBox &box, const glm::vec3 & cameraPos, const glm::vec3& targetPos);

        glm::vec3 getTarget() const;
        glm::vec3 getPosition() const;

    private:
        BoundingBox m_FitBox;
        glm::vec3 m_CameraPos;
        glm::vec3 m_CameraLookAt;
    };

} // OBase

#endif //OPENGL_CAMERAFUNC_H
