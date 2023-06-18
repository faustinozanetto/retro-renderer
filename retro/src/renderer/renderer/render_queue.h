#pragma once

#include "render_command.h"
#include <vector>

namespace retro::renderer
{
    struct render_queue
    {
        std::vector<render_command> command_queue;
    };
}
