#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <RenderBase/Core/Layer.h>

namespace OBase
{
    class DepthPeelingLayer final : public Layer
    {
    public:

        DepthPeelingLayer(DepthPeelingLayer&) = delete;
        DepthPeelingLayer(const DepthPeelingLayer&&) = delete;
        DepthPeelingLayer& operator=(DepthPeelingLayer&) = delete;
        DepthPeelingLayer& operator=(const DepthPeelingLayer&&) = delete;

        explicit DepthPeelingLayer(const std::string& name);

        ~DepthPeelingLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
    };

}

#endif //OPENGL_DEPTHPEELINGLAYER_H
