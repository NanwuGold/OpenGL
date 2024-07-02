#include <RenderBase/Core/OGLBase.h>
#include <iostream>
#include "DualDepthPeelingLayer.h"
#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{
    class DualDepthPeelingApp : public Application
    {
    public:
        explicit DualDepthPeelingApp(const std::string &name = "DualDepthPeelingApp")
                : OBase::Application(name)
        {
            PushLayer(CreateRef<DualDepthPeelingLayer>("DualDepthPeeling Layer"));
        }

        DualDepthPeelingApp(DualDepthPeelingApp &) = delete;
        DualDepthPeelingApp(const DualDepthPeelingApp &&) = delete;
        DualDepthPeelingApp &operator=(DualDepthPeelingApp &) = delete;
        DualDepthPeelingApp &operator=(const DualDepthPeelingApp &&) = delete;
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<DualDepthPeelingApp>();
    }
}