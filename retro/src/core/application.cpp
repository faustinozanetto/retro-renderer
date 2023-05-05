#include "rtpch.h"

#include "application.h"

#include "renderer/renderer/renderer.h"

namespace retro::core
{
    application *application::s_instance = nullptr;

    application::application()
    {
        logging::logger::initialize();
        RT_TRACE("Retro Renderer | Application initialization started.");
        s_instance = this;
        m_window = std::make_shared<renderer::window>(1280, 720, "Retro Renderer");
        renderer::renderer::initialize();
    }

    application::~application()
    {
    }

    void application::main_loop()
    {
        while (!renderer::renderer::get_window_should_close())
        {
            renderer::renderer::set_clear_color({0.15f, 0.15f, 0.15f, 1.0f});
            renderer::renderer::clear_screen();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }
}
