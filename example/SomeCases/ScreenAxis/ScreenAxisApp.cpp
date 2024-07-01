#include <RenderBase/Core/OGLBase.h>
#include <iostream>
#include "ScreenAxisAppLayer.h"
#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{
    class ScreenAxisApp : public Application
    {
    public:
        explicit ScreenAxisApp(const std::string &name = "DualDepthPeelingApp")
                : OBase::Application(name)
        {
            PushLayer(CreateRef<ScreenAxisAppLayer>("DualDepthPeeling Layer"));
        }

        ScreenAxisApp(ScreenAxisApp &) = delete;
        ScreenAxisApp(const ScreenAxisApp &&) = delete;
        ScreenAxisApp &operator=(ScreenAxisApp &) = delete;
        ScreenAxisApp &operator=(const ScreenAxisApp &&) = delete;
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<ScreenAxisApp>();
    }
}