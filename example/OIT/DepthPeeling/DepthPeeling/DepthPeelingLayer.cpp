#include "DepthPeelingLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <RenderBase/Render/FrameBuffer.h>

namespace OBase
{
    DepthPeelingLayer::DepthPeelingLayer(const std::string& name)
        :Layer(name)
    {

    }

    DepthPeelingLayer::~DepthPeelingLayer() = default;

    void DepthPeelingLayer::OnAttach()
    {
        Layer::OnAttach();
        // TODO: implement it 
    }

    void DepthPeelingLayer::OnDetach()
    {
        Layer::OnDetach();
        // TODO: implement it 
    }

    void DepthPeelingLayer::OnUpdate(Timestep ts)
    {
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)));




    }
} // OBase