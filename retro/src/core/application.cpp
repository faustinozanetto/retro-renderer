#include "rtpch.h"

#include <filesystem>

#include "application.h"

#include "renderer/renderer/renderer.h"
#include "interfaces/interface.h"

namespace retro::core
{
    application *application::s_instance = nullptr;

    application::application(const std::string &working_directory)
    {
        logging::logger::initialize();
        std::filesystem::current_path(working_directory);
        RT_TRACE("Retro Renderer | Application initialization started.");
        s_instance = this;
        m_window = std::make_shared<renderer::window>(1280, 720, "Retro Renderer");
        renderer::renderer::initialize();
        ui::interface::initialize();
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

            on_update();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }
}
