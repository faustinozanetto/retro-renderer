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
        int glad_result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        RT_ASSERT_MSG(glad_result, "An error ocurred when initializing GLAD!");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
       // glDepthFunc(GL_LESS);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        RT_TRACE("Retro Renderer | Renderer context initialization completed.");
    }
}
