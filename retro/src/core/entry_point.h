#pragma once

#include "core/application.h"

extern retro::core::application *retro::core::create_application();

int main(int argc, char **argv)
{
    auto application = retro::core::create_application();
    application->main_loop();
    delete application;
}