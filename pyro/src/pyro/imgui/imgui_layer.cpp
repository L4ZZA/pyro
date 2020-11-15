#include "pyro_pch.h"
#include "imgui_layer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


#include "pyro/core/application.h"

// temporary
//#include "glad/glad.h"
#include "GLFW/glfw3.h"

pyro::imgui_layer::imgui_layer(
    uint32_t width, uint32_t height, 
    std::string const& name /*= "imgui_layer"*/)
    : layer(width, height, name)
{
	m_imgui = true;
}

void pyro::imgui_layer::on_attach()
{
	PYRO_PROFILE_FUNCTION();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskbarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.f);
    io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    application& app = application::instance();
    auto win = static_cast<GLFWwindow*>(app.window().native_window());

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void pyro::imgui_layer::on_detach()
{
	PYRO_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void pyro::imgui_layer::on_render() const
{

}

void pyro::imgui_layer::on_imgui_render()
{
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
}

void pyro::imgui_layer::on_event(event& e)
{
    if(m_block_events)
    {
        ImGuiIO &io = ImGui::GetIO();
        e.handled |= e.is_in_category(event_category_mouse) & io.WantCaptureMouse;
        e.handled |= e.is_in_category(event_category_keyboard) & io.WantCaptureKeyboard;
    }
}

void pyro::imgui_layer::begin() const
{
	PYRO_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void pyro::imgui_layer::end() const
{
	PYRO_PROFILE_FUNCTION();
    ImGuiIO& io = ImGui::GetIO();
    auto& app = application::instance();
    io.DisplaySize = ImVec2(static_cast<float>(app.window().width()), static_cast<float>(app.window().height()));

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
