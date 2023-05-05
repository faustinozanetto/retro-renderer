#pragma once

#include "core/application.h"

extern retro::core::application *retro::core::create_application();

int main(int argc, char **argv)
{
    const retro::core::application *application = retro::core::create_application();
    // application->run();
    while (true)
    {
    }
    delete application;
}