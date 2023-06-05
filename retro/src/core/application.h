#pragma once

#include "audio/audio_context.h"
#include "renderer/window/window.h"

#include <thread>
#include <functional>
#include <mutex>

// Entry point used in applications.
int main(int argc, char **argv);

#define BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace retro::core
{
    class application
    {
    public:
        application(const std::string &working_directory);
        virtual ~application();

        /* Getters */
        static application &get() { return *s_instance; }
        std::shared_ptr<renderer::window> &get_window() { return m_window; }

        void on_event(events::base_event &event);
        
        void submit_to_main_thread(const std::function<void()>& function);

        virtual void on_update() = 0;
        virtual void on_handle_event(events::base_event &event) = 0;
        virtual bool on_window_resize(events::window_resize_event &resize_event);

    private:
        void main_loop();
        void execute_main_thread();

        std::shared_ptr<renderer::window> m_window;
        std::shared_ptr<audio::audio_context> m_audio_context;
        float m_frame_delay;
        
        std::vector<std::function<void()>> m_main_thread_queue;
        std::mutex m_main_thread_queue_mutex;

        friend int ::main(int argc, char **argv);
        static application *s_instance;
    };

    application *create_application();
}
