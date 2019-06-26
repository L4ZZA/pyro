#include "pyro_pch.h"
#include "application.h"

#include "platform/opengl/gl_shader.h"
#include "renderer/renderer.h"

pyro::application* pyro::application::s_instance{ nullptr };

pyro::application::application()
{
    PYRO_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    m_window = std::unique_ptr<window>(window::create());
    m_window->event_callback(BIND_EVENT_FN(application::on_event));


    float vertices[3 * 7]
    {
        -.5f, -.5f, .0f,    .8f, .2f, .8f, 1.0f,
         .5f, -.5f, .0f,    .2f, .3f, .8f, 1.0f,
         .0f,  .5f, .0f,    .8f, .8f, .2f, 1.0f,
    };

    std::shared_ptr<vertex_buffer> vb(vertex_buffer::create(vertices, sizeof(vertices)));

    buffer_layout layout{
        {e_shader_data_type::float3, "a_position"},
        {e_shader_data_type::float4, "a_color"},
    };
    vb->layout(layout);

    uint32_t indices[3]{ 0,1,2 };
    std::shared_ptr<index_buffer> ib(index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_vertex_array.reset(vertex_array::create());
    m_vertex_array->add_buffer(vb);
    m_vertex_array->add_buffer(ib);


    float rect_vertices[]
    {
        -0.75f, -0.75f, 0.0f, 
         0.75f, -0.75f, 0.0f, 
         0.75f,  0.75f, 0.0f, 
        -0.75f,  0.75f, 0.0f, 
    };

    const std::shared_ptr<vertex_buffer> rect_vb(vertex_buffer::create(rect_vertices, sizeof(rect_vertices)));

    uint32_t rect_indices[]{ 0,1,2, 2,3,0 };
    const std::shared_ptr<index_buffer> rect_ib(index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t)));

    rect_vb->layout({
        {e_shader_data_type::float3, "a_position"},
    });

    m_rect_va.reset(vertex_array::create());
    m_rect_va->add_buffer(rect_vb);
    m_rect_va->add_buffer(rect_ib);

    const std::string vertex_shader = R"(
        #version 430

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec4 a_color;

        out vec3 v_position;
        out vec4 v_color;

        void main()
        {
            v_position = a_position;
            v_color = a_color;
            gl_Position = vec4(a_position, 1.0);
        }
    )";

    const std::string fragment_shader = R"(
        #version 430

        layout(location = 0) out vec4 o_color;

        in vec3 v_position;
        in vec4 v_color;

        void main()
        {
            o_color = v_color;
        }
    )";

    const std::string rect_vertex_shader = R"(
        #version 430

        layout(location = 0) in vec3 a_position;

        out vec3 v_position;

        void main()
        {
            v_position = a_position;
            gl_Position = vec4(a_position, 1.0);
        }
    )";

    const std::string rect_fragment_shader = R"(
        #version 430

        layout(location = 0) out vec4 o_color;

        in vec3 v_position;

        void main()
        {
            o_color = vec4(.9f, .1f, .6f, 1.f);
        }
    )";

    m_shader.reset(new gl_shader(vertex_shader, fragment_shader));
    m_blue_shader.reset(new gl_shader(rect_vertex_shader, rect_fragment_shader));
}

pyro::application::~application()
{

}

void pyro::application::run()
{
    while (m_running)
    {

        render_command::clear_color({ 0.1f, 0.1f, 0.1f, 1.f });
        render_command::clear();

        renderer::begin_scene();
        
        m_blue_shader->bind();
        renderer::submit(m_rect_va);
        
        m_shader->bind();
        renderer::submit(m_vertex_array);
        
        renderer::end_scene();

        for (auto* layer : m_layers_stack)
        {
            layer->on_update();

            if (layer->is_imgui())
            {
                auto imgui_layer = static_cast<pyro::imgui_layer*>(layer);
                imgui_layer->begin();
                imgui_layer->on_imgui_render();
                imgui_layer->end();
            }
        }

        m_window->on_update();
    }
}

void pyro::application::on_event(event& p_event)
{
    event_dispatcher dispatcher(p_event);
    dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));

    //PYRO_CORE_TRACE("{0}", p_event);

    // events are executed from top of the stack to bottom (aka end to start of the list)
    for (auto it = m_layers_stack.end(); it != m_layers_stack.begin(); )
    {
        (*--it)->on_event(p_event);
        // stop event propagation to next layer if flagged as handled
        if (p_event.handled)
            break;
    }
}

void pyro::application::push_layer(layer* p_layer)
{
    m_layers_stack.push_layer(p_layer);
    p_layer->on_attach();
}

void pyro::application::push_overlay(layer* p_overlay)
{
    m_layers_stack.push_overlay(p_overlay);
    p_overlay->on_attach();
}

bool pyro::application::on_window_close(window_closed_event& p_event)
{
    m_running = false;

    return true;
}
