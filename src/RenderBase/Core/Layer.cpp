#include "Layer.h"

namespace OBase
{
    Layer::Layer(std::string name)
    :m_DebugName(name)
    {

    }

    Layer::~Layer()
    {

    }

    void Layer::OnAttach()
    {

    }

    void Layer::OnDetach()
    {

    }

    void Layer::OnUpdate(Timestep ts)
    {

    }

    void Layer::OnImGuiRender()
    {

    }

    const std::string &Layer::GetName() const
    {
        return m_DebugName;
    }
} // OBase