#include "BoundingBox.h"

namespace OBase
{
    BoundingBox::BoundingBox(const glm::dvec3 &point1, const glm::dvec3 &point2)
        : m_Min(point1), m_Max(point2)
    {
    }

    glm::vec3 BoundingBox::center() const
    {
        return 0.5 * (m_Min + m_Max);
    }

    void BoundingBox::expandBox(const BoundingBox &otherBox)
    {
        auto &minx = m_Min.x;
        auto &miny = m_Min.y;
        auto &minz = m_Min.z;

        auto &maxx = m_Max.x;
        auto &maxy = m_Max.y;
        auto &maxz = m_Max.z;

        {
            const auto otherStart = otherBox.m_Min;
            minx = minx < otherStart.x ? minx : otherStart.x;
            miny = miny < otherStart.y ? miny : otherStart.y;
            minz = minz < otherStart.z ? minz : otherStart.z;
        }
        {
            const auto otherEnd = otherBox.m_Max;
            maxx = maxx > otherEnd.x ? maxx : otherEnd.x;
            maxy = maxy > otherEnd.y ? maxy : otherEnd.y;
            maxz = maxz > otherEnd.z ? maxz : otherEnd.z;
        }
    }

    bool BoundingBox::contains(const glm::dvec3& point) const
    {
        const auto x = point.x > m_Min.x && point.x < m_Max.x;
        const auto y = point.y > m_Min.y && point.y < m_Max.y;
        const auto z = point.z > m_Min.z && point.z < m_Max.z;
        return x && y && z;
    }

    void BoundingBox::reset()
    {
        m_Min = glm::dvec3{std::numeric_limits<double>::max()};
        m_Max = glm::dvec3{std::numeric_limits<double>::lowest()};
    }
} // OBase