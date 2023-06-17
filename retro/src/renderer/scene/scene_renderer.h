#pragma once

#include <camera/camera.h>
#include <renderer/buffers/vertex_array_object.h>
#include <renderer/shaders/shader.h>
#include <renderer/frame_buffer/frame_buffer.h>
#include <renderer/renderer/renderer.h>

#include <glm/glm.hpp>

namespace retro::renderer
{
    struct scene_renderer_data
    {
        std::shared_ptr<camera::camera> camera;

        /* Geometry Pass */
        std::shared_ptr<frame_buffer> geometry_fbo;
        std::shared_ptr<shader> geometry_shader;
    };

    class scene_renderer
    {
    public:
        static void initialize();

        static void begin_render(const std::shared_ptr<camera::camera> &camera);
        static void end_render();

        static void geometry_pass();

    private:
        static scene_renderer_data s_data;
    };
}