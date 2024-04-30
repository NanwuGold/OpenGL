#include "Application.h"
#include "RenderBase/Event/ApplicationEvent.h"
#include "RenderBase/Event/Event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OBase
{
    std::unique_ptr<Application, Application::ApplicationDeleter> Application::s_Instance = nullptr;

    Application::Application(const std::string &name)
    {
        m_Window = std::shared_ptr<Window>(Window::Create(WindowProps(name)));

        m_Window->SetEventCallBack([this](auto &&v) {
            OnEvent(std::forward<decltype(v)>(v));
        });

        m_Window->SetVSync(true);
        s_Instance = std::unique_ptr<Application, ApplicationDeleter>(this, ApplicationDeleter());
    }

    void Application::Run()
    {
        while (m_Running)
        {
            const auto time = static_cast<float>(glfwGetTime()); /// Platform::GetTime
            const Timestep timeStep(time - m_LastFrameTime);
            m_LastFrameTime = time;

            for (const auto &layer: m_LayerStack)
            {
                layer->OnUpdate(timeStep);
            }

            ///TODO: update imGui ： make it as submodule

            m_Window->OnUpdate();
        }
    }

    Application::~Application()
    {
        glfwTerminate();
        m_Window.reset();
    }

    void Application::PushLayer(const Ref<Layer> &layer)
    {
        layer->OnAttach();
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverLay(const Ref<Layer> &overlay)
    {
        overlay->OnAttach();
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>([this](auto &&event) {
            OnResizeEvent(std::forward<decltype(event)>(event));
            return true;
        });

        dispatcher.Dispatch<WindowCloseEvent>([this](auto &&event)
        {
            OnCloseEvent(std::forward<decltype(event)>(event));
            return true;
        });

    }

    void Application::OnCloseEvent(const WindowCloseEvent &event)
    {
        m_Running = false;
    }
} // OBase