#pragma once

#include "camera/camera.h"
#include "renderer/buffers/vertex_array_object.h"
#include "renderer/shaders/shader.h"
#include "renderer/frame_buffer/frame_buffer.h"
#include "renderer/buffers/uniform_buffer_object.h"
#include "renderer/debug/debug_renderer.h"
#include "renderer/renderer/render_queue.h"

#include <glm/glm.hpp>

namespace retro::renderer
{
    struct camera_data
    {
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
        glm::vec3 position;
    };

    struct bloom_mip_data
    {
        glm::ivec2 size;
        std::shared_ptr<texture> texture;
    };

    struct debug_pass_data
    {
        uint32_t max_quads = 20000;
        uint32_t max_vertices = max_quads * 4;

        std::shared_ptr<vertex_array_object> line_vao;
        std::shared_ptr<vertex_buffer_object> line_vbo;
        
        uint32_t line_vertex_count = 0;
        debug_line_vertex* line_vertex_buffer_base = nullptr;
        debug_line_vertex* line_vertex_buffer_ptr = nullptr;
    };

    struct scene_renderer_data
    {
        camera_data camera_data;
        std::shared_ptr<camera::camera> camera;
        std::shared_ptr<uniform_buffer_object> camera_ubo;

        render_queue render_queue;
        
        /* Common */
        std::shared_ptr<vertex_array_object> screen_vao;

        /* Geometry Pass */
        std::shared_ptr<frame_buffer> geometry_fbo;
        std::shared_ptr<shader> geometry_shader;

        /* Lighting Pass */
        std::shared_ptr<frame_buffer> lighting_fbo;
        std::shared_ptr<shader> lighting_shader;

        /* Bloom Pass */
        int bloom_sample_count;
        std::shared_ptr<frame_buffer> bloom_fbo;
        std::shared_ptr<shader> bloom_down_sample_shader;
        std::shared_ptr<shader> bloom_up_sample_shader;
        std::shared_ptr<shader> bloom_composition_shader;
        std::vector<bloom_mip_data> bloom_mips;
        float bloom_filter_radius;

        /* FXAA Pass */
        std::shared_ptr<frame_buffer> fxaa_fbo;
        std::shared_ptr<shader> fxaa_shader;

        /* Debug Pass */
        debug_pass_data debug_pass_data;
        std::shared_ptr<frame_buffer> debug_fbo;
        std::shared_ptr<shader> debug_lines_shader;

        /* Final Pass */
        std::shared_ptr<frame_buffer>
            final_fbo;
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

        static void update_camera_data();
        static void update_camera_buffer();

        static void collect_render_commands();
        static void geometry_pass();
        static void lighting_pass();
        static void bloom_pass();
        static void fxaa_pass();
        static void final_pass();
        static void debug_pass();

        static void on_window_resize(const glm::ivec2& window_size);

    private:
        static void setup_camera();
        static void setup_screen_vao();
        static void setup_geometry_pass();
        static void setup_lighting_pass();
        static void setup_bloom_pass();
        static void setup_fxaa_pass();
        static void setup_debug_pass();
        static void setup_final_pass();

        static scene_renderer_data s_data;
    };
}
