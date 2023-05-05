#include "rtpch.h"
#include "renderer.h"

#include "core/application.h"
#include "renderer/context/renderer_context.h"

namespace retro::renderer
{
    void renderer::initialize()
    {
        core::application &application = core::application::get();
        renderer_context::initialize(application.get_window());
    }
}
