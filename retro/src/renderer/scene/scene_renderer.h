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

        /* Common */
        std::shared_ptr<vertex_array_object> screen_vao;

        /* Geometry Pass */
        std::shared_ptr<frame_buffer> geometry_fbo;
        std::shared_ptr<shader> geometry_shader;

        /* Final Pass */
        std::shared_ptr<frame_buffer> final_fbo;
        std::shared_ptr<shader> final_shader;
    };

    class scene_renderer
    {
    public:
        static void initialize();

        /* Getters */
        static uint32_t get_final_render_target();

        /* Functions */
        static void begin_render(const std::shared_ptr<camera::camera> &camera);
        static void end_render();

        static void geometry_pass();
        static void final_pass();

    private:
        static void setup_screen_vao();
        static void setup_geometry_pass();
        static void setup_final_pass();

        static scene_renderer_data s_data;
    };
}