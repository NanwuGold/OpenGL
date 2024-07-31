#include "Application.h"

#include <GLFW/glfw3.h>
#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Event/ApplicationEvent.h>
#include <RenderBase/Event/Event.h>

namespace OBase
{
    std::unique_ptr<Application, Application::ApplicationDeleter> Application::s_Instance = nullptr;

    Application::Application(const std::string & name)
    : m_imGuiLayer(CreateRef<ImGuiLayer>("ImGui Layer"))
    {
        m_Window = std::shared_ptr<Window>(Window::Create(WindowProps(name)));

        m_Window->SetEventCallBack([this](auto &&v) {
            OnEvent(std::forward<decltype(v)>(v));
        });

        m_Window->SetVSync(true);
        s_Instance = std::unique_ptr<Application, ApplicationDeleter>(this, ApplicationDeleter());

        PushOverLay(m_imGuiLayer);
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

            m_imGuiLayer->Begin();
            for (const auto &layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_imGuiLayer->End();

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
            return false;
        });

        dispatcher.Dispatch<WindowCloseEvent>([this](auto &&event)
        {
            OnCloseEvent(std::forward<decltype(event)>(event));
            return false;
        });


        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.handle())
            {
                break;
            }
        }

    }

    void Application::OnCloseEvent(const WindowCloseEvent &event)
    {
        m_Running = false;
    }

    void Application::OnResizeEvent(const WindowResizeEvent& event)
    {
        
    }

} // OBase 