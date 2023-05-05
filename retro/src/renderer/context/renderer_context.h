#pragma once

#include "renderer/window/window.h"

namespace retro::renderer
{
    class renderer_context
    {
    public:
        static void initialize(const std::shared_ptr<window> &window);

    private:
    };
}