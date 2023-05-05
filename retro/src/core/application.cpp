#include "rtpch.h"

#include "application.h"

#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader.h"
#include "renderer/shaders/shader_loader.h"

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

        const std::string &shader_contents = renderer::shader_loader::read_shader_from_file("../../resources/shaders/test.rrs");
        const auto& shader_sources = renderer::shader_loader::parse_shader_source(shader_contents);
        const std::shared_ptr<renderer::shader> &shader = std::make_shared<renderer::shader>(shader_sources);
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
