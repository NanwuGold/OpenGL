#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include "UniformBufferLayer.h"

namespace OBase
{
    class UniformBufferApp final : public Application
    {
    public:
        UniformBufferApp(UniformBufferApp&) = delete;
        UniformBufferApp(const UniformBufferApp&&) = delete;
        UniformBufferApp& operator=(UniformBufferApp&) = delete;
        UniformBufferApp& operator=(const UniformBufferApp&&) = delete;

        explicit UniformBufferApp(const std::string & name = "Uniform Buffer")
                :Application(name)
        {
             const auto renderLayer = std::make_shared<UniformBufferLayer>("UniformBufferApp Render Layer");
             PushLayer(renderLayer);
        }

        ~UniformBufferApp() override = default;

    protected:
        void OnCloseEvent(const WindowCloseEvent& event) override
        {
            Application::OnCloseEvent(event);
        }

        void OnResizeEvent(const WindowResizeEvent& event) override
        {
            Application::OnResizeEvent(event);
        }
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<UniformBufferApp>("UniformBufferApp Application");
    }

}



