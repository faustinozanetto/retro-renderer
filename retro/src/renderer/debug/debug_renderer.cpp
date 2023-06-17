#include "rtpch.h"
#include "debug_renderer.h"

#include "renderer/shaders/shader_loader.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	debug_renderer_data debug_renderer::s_data = {};

	void debug_renderer::initialize()
	{
		// Setup lines
		s_data.line_vertex_count = 0;
		s_data.line_vertex_buffer_ptr = nullptr;
		s_data.line_vertex_buffer_base = nullptr;
		s_data.lines_shader = shader_loader::load_shader_from_file("resources/shaders/debug_line.rrs");
		s_data.line_vao = std::make_shared<vertex_array_object>();
		s_data.line_vao->bind();
		s_data.line_vbo = std::make_shared<
			vertex_buffer_object>(vertex_buffer_object_target::arrays);
		s_data.line_vbo->set_data(vertex_buffer_object_usage::dynamic_draw, s_data.max_vertices * sizeof(debug_line_vertex), nullptr);

		std::initializer_list<vertex_buffer_layout_entry>
			layout_elements = {
				{"a_pos", vertex_buffer_entry_type::vec_float3, false},
				{"a_color", vertex_buffer_entry_type::vec_float3, false} };

		std::shared_ptr<vertex_buffer_layout_descriptor> vbo_layout_descriptor = std::make_shared<
			vertex_buffer_layout_descriptor>(layout_elements);
		s_data.line_vbo->set_layout_descriptor(vbo_layout_descriptor);

		s_data.line_vao->add_vertex_buffer(s_data.line_vbo);
		s_data.line_vertex_buffer_base = new debug_line_vertex[s_data.max_vertices];
	}

	void debug_renderer::begin_render(const std::shared_ptr<camera::camera>& camera)
	{
		s_data.view_matrix = camera->get_view_matrix();
		s_data.projection_matrix = camera->get_projection_matrix();
		s_data.line_vertex_count = 0;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;
	}

	void debug_renderer::end_render()
	{
		if (s_data.line_vertex_count)
		{
			uint32_t size = (uint32_t)((uint8_t*)s_data.line_vertex_buffer_ptr - (uint8_t*)s_data.line_vertex_buffer_base);
			s_data.line_vbo->set_data(vertex_buffer_object_usage::dynamic_draw, size, s_data.line_vertex_buffer_base);

			s_data.lines_shader->bind();
			s_data.line_vao->bind();
			s_data.lines_shader->set_mat4("u_view", s_data.view_matrix);
			s_data.lines_shader->set_mat4("u_projection", s_data.projection_matrix);
			renderer::submit_arrays(GL_LINES, s_data.line_vertex_count);
			s_data.line_vao->un_bind();
			s_data.lines_shader->un_bind();
		}
	}

	void debug_renderer::submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color)
	{
		//s_data.debug_lines.emplace_back(debug_line(point_a, point_b, color));

		s_data.line_vertex_buffer_ptr->position = point_a;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr++;

		s_data.line_vertex_buffer_ptr->position = point_b;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr++;

		s_data.line_vertex_count += 2;
	}

	void debug_renderer::submit_bounding_box(const math::bounding_box& bounding_box, const glm::vec3& color)
	{
		const glm::vec3& min = bounding_box.get_min_extents();
		const glm::vec3& max = bounding_box.get_max_extents();

		// Submit the lines that make up the bounding box
		submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
		submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
		submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color);
		submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
		submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
		submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
		submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color);
		submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color);
		submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);
		submit_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
		submit_line(glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), color);
		submit_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
	}
}