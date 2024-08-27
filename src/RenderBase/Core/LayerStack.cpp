#include "LayerStack.h"

namespace OBase
{
    LayerStack::LayerStack()
        : m_LayerInsertIndex(0)
    {
    }

    LayerStack::~LayerStack()
    {
        m_Layers.clear();
    }

    void LayerStack::PushLayer(const Ref<Layer>& layer)
    {
        m_Layers.emplace(m_Layers.begin(), layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(const Ref<Layer>& layer)
    {
        m_Layers.emplace_back(layer);
    }

    [[maybe_unused]] void LayerStack::PopLayer(const Ref<Layer>& layer)
    {
        const auto res = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (res != m_Layers.end())
        {
            m_Layers.erase(res);
            m_LayerInsertIndex--;
        }
    }

    [[maybe_unused]] void LayerStack::PopOverlay(const Ref<Layer>& layer)
    {
        const auto res = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (res != m_Layers.end())
        {
            m_Layers.erase(res);
        }
    }
}