#include <pyro.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "imgui.h"
#include "pyro/events/key_event.h"

//glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
//{
//    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//    return Projection * View * Model;
//}

class example_layer : public pyro::imgui_layer
{
public:
    example_layer()
    {
    }

    void on_update() override
    {
        if(pyro::input::key_pressed(pyro::key_codes::KEY_TAB))
            PYRO_INFO("Tab pressed (poll)");
        //PYRO_INFO("ExampleLayer::on_update()");
    }

    void on_imgui_render() override
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Begin("Test");
        ImGui::Text("Rendered text with ImGui.");
        ImGui::End();
    }


    void on_event(pyro::event& p_event) override
    {
        if(p_event.event_type() == pyro::event_type_e::key_pressed)
        {
            auto& e = dynamic_cast<pyro::key_pressed_event&>(p_event);
            if(e.key_code() == pyro::key_codes::KEY_TAB)
                PYRO_TRACE("Tab pressed (event)");
            PYRO_TRACE("{0}", static_cast<char>(e.key_code()));
        }
    }

};


class sandbox_app : public pyro::application
{
public:
    sandbox_app()
    {
        push_layer(new example_layer());
    }

    ~sandbox_app()
    {

    }
};


pyro::application * pyro::create_application()
{
    return new sandbox_app();
}