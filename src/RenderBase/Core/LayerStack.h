#ifndef OBASE_LAYERSTACK_H
#define OBASE_LAYERSTACK_H

#include <RenderBase/Core/Layer.h>
#include <vector>
#include <RenderBase/pointer_ptr.hpp>

namespace OBase
{
    class LayerStack
    {
    public:
        LayerStack();

        ~LayerStack();

        [[maybe_unused]] void PushLayer(const Ref<Layer> &layer);

        [[maybe_unused]] void PushOverlay(const Ref<Layer> &layer);

        [[maybe_unused]] void PopLayer(const Ref<Layer> &layer);

        [[maybe_unused]] void PopOverlay(const Ref<Layer> &layer);

        std::vector<Ref<Layer>>::iterator begin()
        { return m_Layers.begin(); }

        std::vector<Ref<Layer>>::iterator end()
        { return m_Layers.end(); }

    private:
        std::vector<Ref<Layer>> m_Layers;
        unsigned int m_LayerInsertIndex;

    };
}



#endif //HAZEL_LAYERSTACK_H
