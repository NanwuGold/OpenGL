
#ifndef OPENGL_CSMLAYER_H
#define OPENGL_CSMLAYER_H

#include <RenderBase/Core/Layer.h>

namespace OBase
{
    class CSMLayer : public Layer
    {
    public:
        CSMLayer(std::string name = "Layer");

        ~CSMLayer() override;

        void OnAttach() override;

        void OnDetach() override;

        void OnUpdate(Timestep ts) override;

        void OnImGuiRender() override;

    public:


    };

} // OBase

#endif //OPENGL_CSMLAYER_H
