#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>
#include <RenderBase/ToolFunc/OpenGLDebugger.h>

#include "ABufferLayer.h"

namespace OBase
{
    class ABufferApp final : public Application
    {
    public:
        ABufferApp(ABufferApp&) = delete;
        ABufferApp(const ABufferApp&&) = delete;
        ABufferApp& operator=(ABufferApp&) = delete;
        ABufferApp& operator=(const ABufferApp&&) = delete;

        explicit ABufferApp(const std::string & name = "A-Buffer")
                :Application(name)
        {
            /// 注册 debug 输出
            OpenGLDebugger::EnableDebug();

            const auto renderLayer = std::make_shared<ABufferLayer>("A-Buffer Render Layer");
             PushLayer(renderLayer);
        }

        ~ABufferApp() override = default;

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
        return std::make_unique<ABufferApp>("ABufferApp Application");
    }

}



