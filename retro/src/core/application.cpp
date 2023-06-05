﻿#include "rtpch.h"

#include <filesystem>

#include "application.h"

#include "engine_time.h"
#include "assets/asset_manager.h"
#include "renderer/renderer/renderer.h"
#include "interfaces/engine_ui.h"

namespace retro::core
{
    application* application::s_instance = nullptr;

    application::application(const std::string& working_directory)
    {
        logging::logger::initialize();
        std::filesystem::current_path(working_directory);
        RT_TRACE("Retro Renderer | Application initialization started.");
        s_instance = this;
        time::update_time();
        /* Assets */
        assets::asset_manager::initialize();
        /* Window */
        m_window = std::make_shared<renderer::window>(1280, 720, "Retro Renderer");
        m_window->set_event_function(BIND_EVENT_FN(application::on_event));
        /* Renderer */
        renderer::renderer::initialize();
        /* UI */
        ui::engine_ui::initialize();
        /* Audio */
        m_audio_context = std::make_shared<audio::audio_context>();
        time::update_time();
    }

    application::~application()
    {
    }

    void application::on_event(events::base_event& event)
    {
        events::event_dispatcher dispatcher(event);
        dispatcher.dispatch<events::window_resize_event>(BIND_EVENT_FN(application::on_window_resize));
        // Call event handle to child application classes
        on_handle_event(event);
    }

    void application::submit_to_main_thread(const std::function<void()>& function)
    {
        std::scoped_lock<std::mutex> lock(m_main_thread_queue_mutex);
        m_main_thread_queue.emplace_back(function);
    }

    void application::main_loop()
    {
        float frame_time = 0;
        float accumulated_time = 0;
        while (!renderer::renderer::get_window_should_close())
        {
            renderer::renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
            renderer::renderer::clear_screen();

            time::update_time();

            if (m_frame_delay > 0.f)
            {
                frame_time -= time::get_delta_time();
                if (frame_time <= 0.f)
                {
                    frame_time = m_frame_delay;
                    time::set_delta_time(m_frame_delay);
                }
                else
                {
                    continue;
                }
            }

            execute_main_thread();

            on_update();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }

    void application::execute_main_thread()
    {
        std::scoped_lock<std::mutex> lock(m_main_thread_queue_mutex);

        for (auto& func : m_main_thread_queue)
            func();

        m_main_thread_queue.clear();
    }

    bool application::on_window_resize(events::window_resize_event& resize_event)
    {
        RT_TRACE("Retro Renderer | Window resized to: {0}x{1}", resize_event.get_size().x, resize_event.get_size().y);
        renderer::renderer::set_viewport_size(resize_event.get_size());
        return false;
    }
}
