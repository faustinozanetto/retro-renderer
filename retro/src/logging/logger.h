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
#define RT_SEPARATOR(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::trace, "==========================================================================================")
#define RT_INFO(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::info, __VA_ARGS__)
#define RT_TRACE(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::trace, __VA_ARGS__)
#define RT_WARN(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::warn, __VA_ARGS__)
#define RT_ERROR(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::err, __VA_ARGS__)
#define RT_CRITICAL(...) SPDLOG_LOGGER_CALL(::retro::logging::logger::get_logger(), spdlog::level::level_enum::critical, __VA_ARGS__)