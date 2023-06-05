#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace retro::logging
{
    class logger
    {
    public:
        static void initialize();

        /* Getters */
        static std::shared_ptr<spdlog::logger> &get_logger() { return s_logger_instance; }

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