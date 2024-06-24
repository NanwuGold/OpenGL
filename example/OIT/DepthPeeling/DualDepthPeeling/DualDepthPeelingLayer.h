#ifndef OPENGL_DUALDEPTHPEELINGLAYER_H
#define OPENGL_DUALDEPTHPEELINGLAYER_H

#include <glad/glad.h>
#include <RenderBase/Core/Layer.h>

namespace OBase
{
    class DualDepthPeelingLayer :  public OBase::Layer
    {
    public:

        DualDepthPeelingLayer(const std::string & name = "Dual")
                : OBase::Layer(name)
        {

        }

        ~DualDepthPeelingLayer() override
        {

        }

        void OnAttach() override
        {
            Layer::OnAttach();
        }

        void OnDetach() override
        {
            Layer::OnDetach();
        }

        void OnUpdate(Timestep ts) override
        {
            Layer::OnUpdate(ts);

            glClearColor(0.3,0.4,0.5,1.0);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        }

        void OnEvent(Event &event) override
        {
            Layer::OnEvent(event);
        }

        void OnImGuiRender() override
        {

        }
    };
}

#endif //OPENGL_DUALDEPTHPEELINGLAYER_H
