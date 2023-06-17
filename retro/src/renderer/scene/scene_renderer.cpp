#include "rtpch.h"
#include "scene_renderer.h"

#include "renderer/shaders/shader_loader.h"
#include "scene/scene_manager.h"
#include "scene/actors/components/transform_component.h"
#include "scene/actors/components/rendering/model_renderer_component.h"
#include "scene/actors/components/rendering/material_renderer_component.h"
#include "math/transform/transform.h"

namespace retro::renderer
{
	scene_renderer_data scene_renderer::s_data = {};

	void scene_renderer::initialize()
	{
		RT_PROFILE;
		setup_screen_vao();
		setup_camera();
		setup_geometry_pass();
		setup_lighting_pass();
		setup_final_pass();
	}

	uint32_t scene_renderer::get_final_render_target()
	{
		RT_PROFILE;
		return s_data.lighting_fbo->get_attachment_id(0);
	}

	void scene_renderer::begin_render(const std::shared_ptr<camera::camera> &camera)
	{
		RT_PROFILE;
		s_data.camera = camera;

		update_camera_data();
		update_camera_buffer();

		s_data.camera_ubo->bind(0);

		geometry_pass();
		lighting_pass();
		final_pass();
	}

	void scene_renderer::end_render()
	{
		RT_PROFILE;
	}

	void scene_renderer::update_camera_data()
	{
		s_data.camera_data.view_matrix = s_data.camera->get_view_matrix();
		s_data.camera_data.projection_matrix = s_data.camera->get_projection_matrix();
		s_data.camera_data.position = s_data.camera->get_position();
	}

	void scene_renderer::update_camera_buffer()
	{
		s_data.camera_ubo->set_data(sizeof(camera_data), &s_data.camera_data);
	}

	void scene_renderer::geometry_pass()
	{
		RT_PROFILE;
		const auto &current_scene = scene::scene_manager::get().get_active_scene();

		renderer::set_state(renderer_state::blend, false);
		renderer::set_state(renderer_state::depth, true);
		s_data.geometry_fbo->bind();
		renderer::clear_screen();
		s_data.geometry_shader->bind();

		const auto &geometry_pass_view = current_scene->get_actors_registry()->view<scene::transform_component, scene::model_renderer_component, scene::material_renderer_component>();
		for (auto &&[actor, transform_comp, model_renderer_comp, material_renderer_comp] :
			 geometry_pass_view.each())
		{
			const std::shared_ptr<math::transform> &transform = transform_comp.get_transform();
			const std::shared_ptr<material> &material = material_renderer_comp.get_material();
			const std::shared_ptr<model> &model = model_renderer_comp.get_model();

			// Submit to renderer
			s_data.geometry_shader->set_mat4("u_transform", transform->get_transform());
			material->bind(s_data.geometry_shader);
			renderer::submit_model(model);
		}

		s_data.geometry_shader->un_bind();
		s_data.geometry_fbo->un_bind();
		renderer::set_state(renderer_state::depth, false);
	}

	void scene_renderer::lighting_pass()
	{
		RT_PROFILE;
		s_data.lighting_fbo->bind();
		renderer::clear_screen();
		s_data.lighting_shader->bind();
		s_data.lighting_shader->set_vec_float3("u_cam_pos", s_data.camera->get_position());
		s_data.lighting_shader->set_vec_float3("u_directional_light.direction", {1.8f, -5.5f, -3.6f});
		s_data.lighting_shader->set_vec_float3("u_directional_light.color", {6.0f, 6.0f, 6.0f});

		renderer::bind_texture(0, s_data.geometry_fbo->get_attachment_id(0)); // Position
		renderer::bind_texture(1, s_data.geometry_fbo->get_attachment_id(1)); // Albedo
		renderer::bind_texture(2, s_data.geometry_fbo->get_attachment_id(2)); // Normal
		renderer::bind_texture(3, s_data.geometry_fbo->get_attachment_id(3)); // Rough-Metal-AO
		renderer::bind_texture(4, s_data.geometry_fbo->get_attachment_id(4)); // Emissive

		// Render screen quad
		s_data.screen_vao->bind();
		renderer::submit_elements(GL_TRIANGLES, 6);
		s_data.screen_vao->un_bind();

		s_data.lighting_shader->un_bind();
		s_data.lighting_fbo->un_bind();
	}

	void scene_renderer::final_pass()
	{
		RT_PROFILE;
		s_data.final_fbo->bind();
		renderer::clear_screen();
		s_data.final_shader->bind();
		renderer::bind_texture(0, s_data.geometry_fbo->get_attachment_id(1));

		// Render screen quad
		s_data.screen_vao->bind();
		renderer::submit_elements(GL_TRIANGLES, 6);
		s_data.screen_vao->un_bind();

		s_data.final_shader->un_bind();
		s_data.final_fbo->un_bind();
	}

	void scene_renderer::setup_camera()
	{
		s_data.camera_ubo = std::make_shared<uniform_buffer_object>();
		s_data.camera_ubo->initialize(sizeof(camera_data), nullptr);
	}

	void scene_renderer::setup_screen_vao()
	{
		RT_PROFILE;
		std::vector<float> vertices = {
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// top right
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f	// top left
		};

		std::vector<uint32_t> indices = {
			0, 3, 1, // first triangle
			1, 3, 2, // second triangle
		};

		size_t vertex_buffer_size = vertices.size() * sizeof(&vertices[0]);
		size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

		s_data.screen_vao = std::make_shared<retro::renderer::vertex_array_object>();
		std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
			retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

		std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
			retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

		s_data.screen_vao->bind();
		vertices_vbo->bind();
		vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
							   vertices.data());

		index_buffer->bind();
		index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

		std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
			layout_elements = {
				{"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
				{"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
			};

		std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
			vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(
				layout_elements);
		vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

		s_data.screen_vao->add_vertex_buffer(vertices_vbo);
		s_data.screen_vao->set_index_buffer(index_buffer);
		s_data.screen_vao->un_bind();
	}

	void scene_renderer::setup_geometry_pass()
	{
		RT_PROFILE;
		glm::ivec2 viewport_size = renderer::get_viewport_size();
		std::vector<frame_buffer_attachment> geometry_fbo_attachments = {
			//  Position
			{
				texture_internal_format::rgba16f,
				texture_filtering::linear,
				texture_wrapping::clamp_to_edge, viewport_size},
			// Albedo
			{
				texture_internal_format::rgba16f,
				texture_filtering::linear,
				texture_wrapping::clamp_to_edge, viewport_size},
			// Normal
			{
				texture_internal_format::rgba16f,
				texture_filtering::linear,
				texture_wrapping::clamp_to_edge, viewport_size},
			// Roughness Metallic AO
			{
				texture_internal_format::rgba16f,
				texture_filtering::linear,
				texture_wrapping::clamp_to_edge, viewport_size},
			// Emissive
			{
				texture_internal_format::rgba16f,
				texture_filtering::linear,
				texture_wrapping::clamp_to_edge, viewport_size}};
		frame_buffer_attachment geometry_fbo_depth_attachment = {
			texture_internal_format::depth_component32f,
			texture_filtering::linear,
			texture_wrapping::clamp_to_edge, viewport_size};
		s_data.geometry_fbo = std::make_shared<frame_buffer>(
			geometry_fbo_attachments, viewport_size.x, viewport_size.y, geometry_fbo_depth_attachment);
		s_data.geometry_fbo->initialize();
		s_data.geometry_shader = shader_loader::load_shader_from_file("resources/shaders/geometry.rrs");
	}

	void scene_renderer::setup_lighting_pass()
	{
		RT_PROFILE;
		glm::ivec2 viewport_size = renderer::get_viewport_size();
		std::vector<frame_buffer_attachment> final_fbo_attachments = {
			{texture_internal_format::rgba16f,
			 texture_filtering::linear,
			 texture_wrapping::clamp_to_edge, viewport_size},
		};
		s_data.lighting_fbo = std::make_shared<frame_buffer>(
			final_fbo_attachments, viewport_size.x, viewport_size.y);
		s_data.lighting_fbo->initialize();
		s_data.lighting_shader = shader_loader::load_shader_from_file("resources/shaders/pbr.rrs");
	}

	void scene_renderer::setup_final_pass()
	{
		RT_PROFILE;
		glm::ivec2 viewport_size = renderer::get_viewport_size();
		std::vector<frame_buffer_attachment> final_fbo_attachments = {
			{texture_internal_format::rgba16f,
			 texture_filtering::linear,
			 texture_wrapping::clamp_to_edge, viewport_size},
		};
		s_data.final_fbo = std::make_shared<frame_buffer>(
			final_fbo_attachments, viewport_size.x, viewport_size.y);
		s_data.final_fbo->initialize();
		s_data.final_shader = shader_loader::load_shader_from_file("resources/shaders/screen.rrs");
	}
}