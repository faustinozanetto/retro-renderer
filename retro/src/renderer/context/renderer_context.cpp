#include "rtpch.h"
#include "renderer_context.h"

namespace retro::renderer
{
    void renderer_context::initialize(const std::shared_ptr<window> &window)
    {
        RT_TRACE("Retro Renderer | Renderer context initialization started.");
        // Set glfw context
        glfwMakeContextCurrent(window->get_handle());
        // Initialize glad
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            assert(false);

        RT_TRACE("Retro Renderer | Renderer context initialization completed.");
    }
}
