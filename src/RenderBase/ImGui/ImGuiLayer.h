#ifndef OPENGL_IMGUILAYER_H
#define OPENGL_IMGUILAYER_H

#include <RenderBase/Core/Layer.h>

namespace OBase
{
    class ImGuiLayer: public Layer
    {
    public:
        explicit ImGuiLayer(const std::string & name);

        ~ImGuiLayer() override;

        void OnAttach() override;

        void OnDetach() override;

        void OnUpdate(Timestep ts) override;

        void OnEvent(Event &event) override;

        void OnImGuiRender() override;

        void Begin();

        void End();
    };

} // OBase

#endif //OPENGL_IMGUILAYER_H
