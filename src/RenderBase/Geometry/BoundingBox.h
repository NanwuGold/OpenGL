#ifndef OPENGL_BOUNDINGBOX_H
#define OPENGL_BOUNDINGBOX_H

#include <glm/vec3.hpp>

namespace OBase
{
    class BoundingBox
    {
    public:
        BoundingBox() = default;
        BoundingBox(const glm::dvec3 & point1, const glm::dvec3 & point2);

        /**
         * @brief 获取包围盒的中点
         * @return glm::vec3
         */
        [[nodiscard]] glm::vec3 center() const;

        /**
         * @brief 扩展包围盒(计算两个包围盒的顶点可以包到的最大空间)
         * @param otherBox 另一个包围盒
         */
        void expendBox(const BoundingBox & otherBox);

        glm::dvec3 min() const
        {
            return m_Min;
        }

        glm::dvec3 max() const
        {
            return m_Max;
        }


    private:
        glm::dvec3 m_Min{std::numeric_limits<double>::max()};
        glm::dvec3 m_Max{std::numeric_limits<double>::lowest()};
    };

} // OBase

#endif //OPENGL_BOUNDINGBOX_H
