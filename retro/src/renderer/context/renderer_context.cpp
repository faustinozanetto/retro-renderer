#include "rtpch.h"
#include "renderer_context.h"

namespace retro::renderer
{
    void open_gl_message_callback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char *message,
        const void *userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            RT_CRITICAL(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            RT_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            RT_WARN(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            RT_TRACE(message);
            return;
        }

        RT_ASSERT_MSG(false, "Unknown severity level!");
    }

    void renderer_context::initialize(const std::shared_ptr<window> &window)
    {
        RT_PROFILE;
        RT_TRACE("Retro Renderer | Renderer context initialization started.");
        // Set glfw context
        glfwMakeContextCurrent(window->get_handle());
        // Initialize glad
        int glad_result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        RT_ASSERT_MSG(glad_result, "An error ocurred when initializing GLAD!");

#ifdef TRACY_ENABLE
        TracyGpuContext;
#endif

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(open_gl_message_callback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        RT_TRACE("Retro Renderer | Renderer context initialization completed.");
    }
}
