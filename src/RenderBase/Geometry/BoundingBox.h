#ifndef OPENGL_BOUNDINGBOX_H
#define OPENGL_BOUNDINGBOX_H

#include <glm/vec3.hpp>

#include "BoundingBox.h"
#include "BoundingBox.h"

namespace OBase
{
    class BoundingBox
    {
    public:
        BoundingBox() = default;
        BoundingBox(const glm::dvec3 &point1, const glm::dvec3 &point2);

        /**
         * @brief 获取包围盒的中点
         * @return glm::vec3
         */
        [[nodiscard]] glm::vec3 center() const;

        /**
         * @brief 扩展包围盒(计算两个包围盒的顶点可以包到的最大空间)
         * @param otherBox 另一个包围盒
         */
        void expandBox(const BoundingBox &otherBox);

        /**
         * @brief 判断点是否在包围盒内部
         *
         * @param point glm::vec3
         * @return true 在包围盒内部
         * @return false 不在包围盒内部
         */
        bool contains(const glm::dvec3& point) const;

        glm::dvec3 min() const
        {
            return m_Min;
        }

        glm::dvec3 max() const
        {
            return m_Max;
        }

        /**
         * @brief 重置包围盒
         */
        void reset();

    private:
        glm::dvec3 m_Min{std::numeric_limits<double>::max()};
        glm::dvec3 m_Max{std::numeric_limits<double>::lowest()};
    };

} // OBase

#endif // OPENGL_BOUNDINGBOX_H
