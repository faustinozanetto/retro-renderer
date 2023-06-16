#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace retro::logging
{
    class logger
    {
    public:
        static void initialize();

        /* Getters */
        inline static std::shared_ptr<spdlog::logger> &get_logger() { return s_logger_instance; }
        static void add_sink(spdlog::sink_ptr& sink);

    private:
        static std::shared_ptr<spdlog::logger> s_logger_instance;
    };
}

// Logging macros
#define RT_SEPARATOR(...) ::retro::logging::logger::get_logger()->trace("==========================================================================================")
#define RT_INFO(...) ::retro::logging::logger::get_logger()->info(__VA_ARGS__)
#define RT_TRACE(...) ::retro::logging::logger::get_logger()->trace(__VA_ARGS__)
#define RT_WARN(...) ::retro::logging::logger::get_logger()->warn(__VA_ARGS__)
#define RT_ERROR(...) ::retro::logging::logger::get_logger()->error(__VA_ARGS__)
#define RT_CRITICAL(...) ::retro::logging::logger::get_logger()->critical(__VA_ARGS__)