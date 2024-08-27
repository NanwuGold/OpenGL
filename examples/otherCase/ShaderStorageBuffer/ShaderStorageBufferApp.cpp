#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include "ShaderStorageBufferLayer.h"
#include <RenderBase/ToolFunc/OpenGLDebugger.h>

namespace OBase
{
    class ShaderStorageBufferApp final : public Application
    {
    public:
        ShaderStorageBufferApp(ShaderStorageBufferApp&) = delete;
        ShaderStorageBufferApp(const ShaderStorageBufferApp&&) = delete;
        ShaderStorageBufferApp& operator=(ShaderStorageBufferApp&) = delete;
        ShaderStorageBufferApp& operator=(const ShaderStorageBufferApp&&) = delete;

        explicit ShaderStorageBufferApp(const std::string & name = "ShaderStorageBuffer Buffer")
                :Application(name)
        {
             const auto renderLayer = std::make_shared<ShaderStorageBufferLayer>("ShaderStorageBufferApp Render Layer");
             PushLayer(renderLayer);

            /// 注册 debug 输出
            OpenGLDebugger::EnableDebug();
        }

        ~ShaderStorageBufferApp() override = default;

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
        return std::make_unique<ShaderStorageBufferApp>("ShaderStorageBufferApp Application");
    }

}



