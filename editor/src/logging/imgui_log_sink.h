#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace retro::editor
{
    template <typename Mutex>
    class imgui_log_sink : public spdlog::sinks::base_sink<Mutex>
    {
    public:
        explicit imgui_log_sink() {};

        imgui_log_sink(const imgui_log_sink&) = delete;
        imgui_log_sink& operator = (const imgui_log_sink&) = delete;
        virtual ~imgui_log_sink() = default;

        void sink_it_(const spdlog::details::log_msg& msg) override {
			spdlog::memory_buf_t formatted_buffer;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted_buffer);

			std::string str = fmt::to_string(formatted_buffer);

			auto app = dynamic_cast<editor_app*>(&core::application::get());
			auto console_panel = app->get_main_layer()->get_console_panel();
			console_panel->add_message(std::make_shared<editor_console_message>(str));
        }

        void flush_() override {

        }
    };
}