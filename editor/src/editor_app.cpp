﻿#include "editor_app.h"

#include "logging/imgui_log_sink.h"

#include <core/entry_point.h>
#include <spdlog/spdlog.h>
#include <mutex>

namespace retro::editor
{
    editor_app::editor_app() : application("./", {"Retro Engine | Editor", 1920, 1080})
    {
        initialize();
        renderer::renderer::set_vsync_enabled(false);
    }

    editor_app::~editor_app()
    {
    }

    void editor_app::initialize()
    {
        m_main_layer = std::make_shared<editor_main_layer>();
        spdlog::sink_ptr imgui_sink = std::make_shared<imgui_log_sink<std::mutex>>();
        logging::logger::add_sink(imgui_sink);
    }

    void editor_app::on_update()
    {
        m_main_layer->on_update();
    }

    void editor_app::on_handle_event(events::base_event &event)
    {
        events::event_dispatcher dispatcher(event);
        dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(editor_app::on_key_pressed));
        dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(editor_app::on_window_resize));
    }

    bool editor_app::on_window_resize(events::window_resize_event &resize_event)
    {
        return application::on_window_resize(resize_event);
    }

    bool editor_app::on_key_pressed(retro::events::key_pressed_event &key_pressed_event)
    {
        return false;
    }
}

retro::core::application *retro::core::create_application()
{
    return new retro::editor::editor_app();
}
