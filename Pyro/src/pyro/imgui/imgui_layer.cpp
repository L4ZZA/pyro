#include "pyro_pch.h"
#include "imgui_layer.h"

#include "platform/opengl/imgui_opengl_renderer.h"
#include "GLFW/glfw3.h"
#include "pyro/application.h"

// temporary
#include "glad/glad.h"

pyro::imgui_layer::imgui_layer()
    : layer("imgui_layer")
{
}

pyro::imgui_layer::~imgui_layer()
{
}

void pyro::imgui_layer::on_attach()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TEMPORARY!! Should eventually use pyro keycode.
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void pyro::imgui_layer::on_detach()
{
}

void pyro::imgui_layer::on_update()
{

    ImGuiIO& io = ImGui::GetIO();
    auto& app = application::instance();
    io.DisplaySize = ImVec2(app.get_window().width(), app.get_window().height());

    float time = glfwGetTime();
    io.DeltaTime = m_time > 0.0 ? (time - m_time) : (1.f / 60.f);
    m_time = time;

    // NewFrame after GetIO() and maybe also after io.DetltaTime
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void pyro::imgui_layer::on_event(event& p_event)
{
    event_dispatcher dispatcher(p_event);
    dispatcher.dispatch<mouse_button_pressed_event>(BIND_EVENT_FN(imgui_layer::on_mouse_button_pressed_event));
    dispatcher.dispatch<mouse_button_released_event>(BIND_EVENT_FN(imgui_layer::on_mouse_button_released_event));
    dispatcher.dispatch<mouse_moved_event>(BIND_EVENT_FN(imgui_layer::on_mouse_moved_event));
    dispatcher.dispatch<mouse_scrolled_event>(BIND_EVENT_FN(imgui_layer::on_mouse_scrolled_event));
    dispatcher.dispatch<key_pressed_event>(BIND_EVENT_FN(imgui_layer::on_key_pressed_event));
    dispatcher.dispatch<key_released_event>(BIND_EVENT_FN(imgui_layer::on_key_released_event));
    dispatcher.dispatch<key_typed_event>(BIND_EVENT_FN(imgui_layer::on_key_typed_event));
    dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN(imgui_layer::on_window_resize_event));
}

bool pyro::imgui_layer::on_mouse_button_pressed_event(mouse_button_pressed_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[e.mouse_button()] = true;

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_mouse_button_released_event(mouse_button_released_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[e.mouse_button()] = false;

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_mouse_moved_event(mouse_moved_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos = ImVec2(e.x(), e.y());

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_mouse_scrolled_event(mouse_scrolled_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += e.x_offset();
    io.MouseWheel += e.y_offset();

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_key_pressed_event(key_pressed_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.KeysDown[e.key_code()] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    // cmd/super/window key, depending on OS
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_key_released_event(key_released_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.KeysDown[e.key_code()] = false;

    // handled flag
    return false;
}

bool pyro::imgui_layer::on_key_typed_event(key_typed_event & e)
{
    ImGuiIO &io = ImGui::GetIO();
    int key_code = e.key_code();
    if(key_code > 0 && key_code < 0x10000)
        io.AddInputCharacter((unsigned short)key_code);
    return false;
}

bool pyro::imgui_layer::on_window_resize_event(window_resize_event& e)
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(e.width(), e.height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    // Temporary TODO: abstract
    glViewport(0,0, e.width(), e.height());

    // handled flag
    return false;
}
