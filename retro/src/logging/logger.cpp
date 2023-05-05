#include "rtpch.h"
#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace retro::logging
{
    std::shared_ptr<spdlog::logger> logger::s_logger_instance;

    void logger::initialize()
    {

        std::vector<spdlog::sink_ptr> sinks;
        sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("retro.log", true));

        sinks[0]->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");
        sinks[1]->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");

        s_logger_instance = std::make_shared<spdlog::logger>("RETRO", begin(sinks), end(sinks));
        spdlog::register_logger(s_logger_instance);
        s_logger_instance->set_level(spdlog::level::trace);
        s_logger_instance->flush_on(spdlog::level::trace);
    }
}