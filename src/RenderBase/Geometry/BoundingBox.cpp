#include "BoundingBox.h"

namespace OBase
{
    BoundingBox::BoundingBox(const glm::dvec3 &point1, const glm::dvec3 &point2)
    : m_min(point1)
    , m_max(point2)
    {

    }

    glm::vec3 BoundingBox::center() const
    {
        return 0.5 * (m_min + m_max);
    }

    void BoundingBox::expendBox(const BoundingBox &otherBox)
    {
        auto & minx = m_min.x;
        auto & miny = m_min.y;
        auto & minz = m_min.z;

        auto & maxx = m_max.x;
        auto & maxy = m_max.y;
        auto & maxz = m_max.z;

        const auto otherStart = otherBox.m_min;
        {
            minx = minx < otherStart.x ? minx : otherStart.x;
            miny = miny < otherStart.y ? miny : otherStart.y;
            minz = minz < otherStart.z ? minz : otherStart.z;
        }
        const auto otherEnd = otherBox.m_max;
        {
            maxx = maxx > otherEnd.x ? maxx : otherEnd.x;
            maxy = maxy > otherEnd.y ? maxy : otherEnd.y;
            maxz = maxz > otherEnd.z ? maxz : otherEnd.z;
        }
    }
} // OBase