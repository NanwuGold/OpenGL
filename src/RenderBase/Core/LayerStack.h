#ifndef OBASE_LAYERSTACK_H
#define OBASE_LAYERSTACK_H

#include <vector>

#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Core/Layer.h>

namespace OBase
{
    class LayerStack final
    {
    public:

        LayerStack(LayerStack&) = delete;
        LayerStack(LayerStack&&) = delete;

        LayerStack& operator=(LayerStack&) = delete;
        LayerStack& operator=(LayerStack&&) = delete;

        LayerStack();
        ~LayerStack();

        void PushLayer(const Ref<Layer> &layer);

        void PushOverlay(const Ref<Layer> &layer);

        void PopLayer(const Ref<Layer> &layer);

        void PopOverlay(const Ref<Layer> &layer);

        std::vector<Ref<Layer>>::iterator begin()
        { return m_Layers.begin(); }

        std::vector<Ref<Layer>>::iterator end()
        { return m_Layers.end(); }

    private:
        std::vector<Ref<Layer>> m_Layers;
        unsigned int m_LayerInsertIndex;

    };
}



#endif //OBASE_LAYERSTACK_H
