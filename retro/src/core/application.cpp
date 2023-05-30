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
        m_window->set_event_function([this](auto &&...args) -> decltype(auto)
                                     { return application::on_event(std::forward<decltype(args)>(args)...); });
        renderer::renderer::initialize();
     
        ui::interface::initialize();
    }

    application::~application()
    {
    }

    void application::on_event(events::base_event &event)
    {
        events::event_dispatcher dispatcher(event);
        dispatcher.dispatch<events::window_resize_event>([this](auto &&...args) -> decltype(auto)
                                                         { return application::on_window_resize(std::forward<decltype(args)>(args)...); });
        // Call event handle to child application classes
        on_handle_event(event);
    }

    void application::main_loop()
    {
        while (!renderer::renderer::get_window_should_close())
        {
            renderer::renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
            renderer::renderer::clear_screen();

            on_update();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }

    bool application::on_window_resize(events::window_resize_event &resize_event)
    {
        RT_TRACE("Retro Renderer | Window resized to: {0}x{1}", resize_event.get_size().x, resize_event.get_size().y);
        renderer::renderer::set_viewport_size(resize_event.get_size());
        return false;
    }
}
