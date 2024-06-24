#include <RenderBase/Core/OGLBase.h>
#include <iostream>
#include "DualDepthPeelingLayer.h"
#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{
    class DualDepthPeeling : public OBase::Application
    {
    public:
        DualDepthPeeling(DualDepthPeeling &) = delete;

        DualDepthPeeling(const DualDepthPeeling &&) = delete;

        DualDepthPeeling &operator=(DualDepthPeeling &) = delete;

        DualDepthPeeling &operator=(const DualDepthPeeling &&) = delete;

        explicit DualDepthPeeling(const std::string &name = "DualDepthPeeling")
                : OBase::Application(name)
        {
            const auto layer = CreateRef<DualDepthPeelingLayer>("Dual Layer");
            PushLayer(layer);
        }
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<DualDepthPeeling>();
    }

}