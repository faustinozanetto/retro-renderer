#pragma once

#include "core/application.h"
#include "logging/logger.h"
#include "job_system.h"

extern retro::core::application *retro::core::create_application();

int main(int argc, char **argv)
{
    retro::logging::logger::initialize();
    retro::core::job_system::initialize();
    retro::core::job_system::get().start();

    auto application = retro::core::create_application();
    application->main_loop();

    retro::core::job_system::get().stop();

    delete application;
}