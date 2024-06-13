#include <iostream>

#include <RenderBase/Core/Application.h>

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

        }


    };
}