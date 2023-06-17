#pragma once

#include <renderer/buffers/vertex_array_object.h>
#include <renderer/shaders/shader.h>
#include <camera/camera.h>
#include "math/bounding_box.h"

#include <glm/glm.hpp>

namespace retro::renderer
{
    struct debug_line
    {
        glm::vec3 point_a;
        glm::vec3 point_b;
        glm::vec3 color;

        debug_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color) : point_a(point_a), point_b(point_b), color(color) {}
    };

	struct debug_line_vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

    struct debug_renderer_data
    {
        uint32_t max_quads = 20000;
        uint32_t max_vertices = max_quads * 4;

        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;

        std::vector<debug_line> debug_lines;
        std::shared_ptr<vertex_array_object> line_vao;
        std::shared_ptr<vertex_buffer_object> line_vbo;
        std::shared_ptr<shader> lines_shader;

		uint32_t line_vertex_count = 0;
        debug_line_vertex* line_vertex_buffer_base = nullptr;
        debug_line_vertex* line_vertex_buffer_ptr = nullptr;
    };

    class debug_renderer
    {
    public:
        static void initialize();

        static void begin_render(const std::shared_ptr<camera::camera>& camera);
        static void end_render();

        static void submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color);
        static void submit_bounding_box(const math::bounding_box& bounding_box, const glm::vec3& color);
    private:
        static debug_renderer_data s_data;
    };
}