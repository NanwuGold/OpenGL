#include "ImGuiLayer.h"

#include <RenderBase/Core/Application.h>
#include <imgui.h>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace OBase
{
    ImGuiLayer::ImGuiLayer(const std::string &name)
    : Layer(name)
    {

    }

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        const auto & app = Application::Get();
        const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init("#version 460");

    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
    }

    void ImGuiLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    void ImGuiLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();
    }

    void ImGuiLayer::Begin() const
    {
        assert(this);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() const
    {
        assert(this);
        ImGuiIO &io = ImGui::GetIO();
        const Application & app = Application::Get();
        const auto &window = app.GetWindow();
        io.DisplaySize = ImVec2(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // OBase