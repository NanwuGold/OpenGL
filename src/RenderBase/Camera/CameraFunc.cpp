#include "CameraFunc.h"

namespace OBase
{
    CameraFunc::CameraFunc(const BoundingBox &box, const glm::vec3 &front)
            : m_fitBox(box)
            , m_front(front)
    {

    }

    glm::vec3 CameraFunc::getPosition()
    {
        auto length = glm::distance(m_fitBox.min(),m_fitBox.max());
        length = length * 0.5 * std::sqrt(2);

        auto frontVec = glm::normalize(m_front);
        auto cameraPos = getTarget() - frontVec * static_cast<float>(length);
        return cameraPos;
    }

    glm::vec3 CameraFunc::getTarget()
    {
        return m_fitBox.center();
    }
} // OBase