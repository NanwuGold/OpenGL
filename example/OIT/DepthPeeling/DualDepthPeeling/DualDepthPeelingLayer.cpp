#include "DualDepthPeelingLayer.h"

namespace OBase
{
    DualDepthPeelingLayer::DualDepthPeelingLayer(const std::string &name)
            : OBase::Layer(name)
    {

    }

    DualDepthPeelingLayer::~DualDepthPeelingLayer()
    {

    }

    void DualDepthPeelingLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("BackGround Color",glm::value_ptr(m_BackgroundColor),ImGuiColorEditFlags_NoAlpha);
        ImGui::End();
    }

    void DualDepthPeelingLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_BackgroundColor));
    }

    void DualDepthPeelingLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    void DualDepthPeelingLayer::OnAttach()
    {

    }

    void DualDepthPeelingLayer::OnDetach()
    {
        //destory resources
    }
}