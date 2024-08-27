#ifndef OPENGL_LAYER_H
#define OPENGL_LAYER_H

#include <string>
#include <RenderBase/Event/Event.h>

#include "Timestep.h"

namespace OBase
{
    class Layer
    {
    public:
        Layer(const Layer&) = default;
        Layer(Layer&&) = default;
        Layer& operator=(const Layer&) = default;
        Layer& operator=(Layer&&) = default;

        explicit Layer(std::string name = "Layer");
        virtual ~Layer();

        /**
         * @brief 层被添加时触发
         */
        [[maybe_unused]] virtual void OnAttach();

        /**
         * @brief 层被移除时触发
         */
        [[maybe_unused]] virtual void OnDetach();

        /**
         * @brief 每一帧更新函数
         * @param ts 时间
         */
        virtual void OnUpdate(Timestep ts);

        /**
         * @brief 更新IMGUI的组件的信息
         */
        virtual void OnImGuiRender();

        /**
         * @brief 分发事件
         * @param event
         */
        virtual void OnEvent(Event &event);

        /**
         * @brief 获取Layer的名字
         * @return std::string
         */
        [[nodiscard]] const std::string &GetName() const;

    protected:
        std::string m_DebugName;
    };

} // OBase

#endif //OPENGL_LAYER_H
