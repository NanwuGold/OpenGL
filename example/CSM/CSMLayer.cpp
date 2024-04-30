#include "CSMLayer.h"
#include <glad/glad.h>

namespace OBase
{
    CSMLayer::CSMLayer(std::string name)
    : Layer(name)
    {

    }

    CSMLayer::~CSMLayer()
    {

    }

    void CSMLayer::OnAttach()
    {
        /// INIT Resources
        /// TODO:






    }

    void CSMLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void CSMLayer::OnUpdate(Timestep ts)
    {
        glClearColor(0.3,0.4,0.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void CSMLayer::OnImGuiRender()
    {

    }
} // OBase