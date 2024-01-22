/**
 * @file   渲染使用的宏的相关的定义
 * @brief  渲染使用的宏的相关的定义
 * @author RanLh
 * @note   none
 */

#ifndef RENDERENUM_H
#define RENDERENUM_H

    namespace shader
    {
        enum class shader_type
        {
            vertex_shader,
            fragment_shader,
            geometry_shader,
            program_shader

        };
    }

#endif //! RENDEREN_ENUM_H
