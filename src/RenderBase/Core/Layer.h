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

        [[maybe_unused]] virtual void OnAttach();
        [[maybe_unused]] virtual void OnDetach();

        virtual void OnUpdate(Timestep ts);

        virtual void OnEvent(Event &event);

        virtual void OnImGuiRender();

        [[nodiscard]] const std::string &GetName() const;

    protected:
        std::string m_DebugName;
    };

} // OBase

#endif //OPENGL_LAYER_H
