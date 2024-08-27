#include "CameraFunc.h"

namespace OBase
{
    CameraFunc::CameraFunc(const BoundingBox& box, const glm::vec3& cameraPos, const glm::vec3& targetPos)
        : m_FitBox(box)
        , m_CameraPos(cameraPos)
        , m_CameraLookAt(targetPos)
    {

    }

    glm::vec3 CameraFunc::getPosition() const
    {
        auto length = glm::distance(m_FitBox.min(),m_FitBox.max());
        length = length * 0.5 * std::sqrt(2);

        
        const auto frontVec = glm::normalize(m_CameraLookAt - m_CameraPos);
        const auto cameraPos = getTarget() - frontVec * static_cast<float>(length);
        return cameraPos;
    }

    glm::vec3 CameraFunc::getTarget() const
    {
        return m_FitBox.center();
    }
} // OBase