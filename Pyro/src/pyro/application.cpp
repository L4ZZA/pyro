#include "pyro_pch.h"
#include "application.h"
#include "glad/glad.h"
#include "pyro/input.h"

pyro::application* pyro::application::s_instance{ nullptr };

pyro::application::application()
{
    PYRO_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    m_window = std::unique_ptr<window>(window::create());
	m_window->event_callback(BIND_EVENT_FN(application::on_event));

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

	float vertices[3 * 3]
	{
		-.5f, -.5f, .0f,
		 .5f, -.5f, .0f,
		 .0f,  .5f, .0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

	unsigned int indices[3]{ 0,1,2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

pyro::application::~application()
{

}

void pyro::application::run()
{
	while(m_running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(m_vertex_array);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		for(auto* layer : m_layers_stack)
		{
            layer->on_update();

			if(layer->is_imgui())
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
