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

        ~UniformBufferApp() override;

    protected:
        void OnCloseEvent(const WindowCloseEvent& event) override;
        void OnResizeEvent(const WindowResizeEvent& event) override;
    };

    UniformBufferApp::~UniformBufferApp() = default;

    void UniformBufferApp::OnCloseEvent(const WindowCloseEvent& event)
    {
        Application::OnCloseEvent(event);
    }

    void UniformBufferApp::OnResizeEvent(const WindowResizeEvent& event)
    {
        Application::OnResizeEvent(event);
    }

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<UniformBufferApp>("UniformBufferApp Application");
    }

}



