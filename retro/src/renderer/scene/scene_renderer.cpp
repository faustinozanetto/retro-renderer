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
		setup_bloom_pass();
		setup_fxaa_pass();
		setup_final_pass();
	}

	uint32_t scene_renderer::get_final_render_target()
	{
		RT_PROFILE;
		return s_data.final_fbo->get_attachment_id(0);
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
		bloom_pass();
		fxaa_pass();
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

	void scene_renderer::bloom_pass()
	{
		RT_PROFILE;
		// 1. Bloom down sample pass
		s_data.bloom_fbo->bind();
		s_data.bloom_down_sample_shader->bind();
		// Bind texture from previous pass
		renderer::bind_texture(0, s_data.lighting_fbo->get_attachment_id(0));
		s_data.bloom_down_sample_shader->set_vec_float2("u_source_res", renderer::get_viewport_size());
		s_data.bloom_down_sample_shader->set_int("u_mip_level", 0);
		for (int i = 0; i < s_data.bloom_mips.size(); i++)
		{
			bloom_mip_data &bloom_mip = s_data.bloom_mips[i];
			renderer::set_viewport_size(bloom_mip.size);
			// Attach bloom mip textue to rbo.
			s_data.bloom_fbo->attach_texture(bloom_mip.texture, GL_FRAMEBUFFER, render_buffer_attachment_type::color, GL_TEXTURE_2D, false, 0);

			// Draw screen quad
			s_data.screen_vao->bind();
			renderer::submit_elements(GL_TRIANGLES, 6);
			s_data.screen_vao->un_bind();

			// Set current mip resolution as srcResolution for next iteration
			s_data.bloom_down_sample_shader->set_vec_float2("u_source_res", bloom_mip.size);
			// Set current mip as texture input for next iteration
			renderer::bind_texture(0, bloom_mip.texture->get_handle_id());
			// Disable Karis average for consequent down samples
			if (i == 0)
			{
				s_data.bloom_down_sample_shader->set_int("u_mip_level", 1);
			}
		}
		s_data.bloom_down_sample_shader->un_bind();

		// 2. Bloom up sample pass
		renderer::set_state(renderer_state::blend, true);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		s_data.bloom_up_sample_shader->bind();
		s_data.bloom_up_sample_shader->set_float("u_filter_radius", s_data.bloom_filter_radius);
		for (int i = s_data.bloom_mips.size() - 1; i > 0; i--)
		{
			bloom_mip_data &bloom_mip = s_data.bloom_mips[i];
			bloom_mip_data &bloom_next_mip = s_data.bloom_mips[i - 1];
			// Bind mip texture
			renderer::bind_texture(0, bloom_mip.texture->get_handle_id());
			renderer::set_viewport_size(bloom_next_mip.size);
			// Attach bloom mip texture to render buffer object.
			s_data.bloom_fbo->attach_texture(bloom_next_mip.texture, GL_FRAMEBUFFER, render_buffer_attachment_type::color, GL_TEXTURE_2D, false, 0);
			// Draw screen quad
			s_data.screen_vao->bind();
			renderer::submit_elements(GL_TRIANGLES, 6);
			s_data.screen_vao->un_bind();
		}

		s_data.bloom_up_sample_shader->un_bind();
		renderer::set_state(renderer_state::blend, false);

		// 3. Bloom composition pass
		s_data.bloom_composition_shader->bind();
		renderer::bind_texture(0, s_data.lighting_fbo->get_attachment_id(0));	  // Lighting
		renderer::bind_texture(1, s_data.bloom_mips[0].texture->get_handle_id()); // Bloom

		// Render screen quad
		s_data.screen_vao->bind();
		renderer::submit_elements(GL_TRIANGLES, 6);
		s_data.screen_vao->un_bind();

		s_data.bloom_composition_shader->un_bind();
		s_data.bloom_fbo->un_bind();
	}

	void scene_renderer::fxaa_pass()
	{
		RT_PROFILE;
		s_data.fxaa_fbo->bind();
		renderer::clear_screen();
		s_data.fxaa_shader->bind();
		renderer::bind_texture(0, s_data.bloom_fbo->get_attachment_id(0)); // Bloom output

		// Render screen quad
		s_data.screen_vao->bind();
		renderer::submit_elements(GL_TRIANGLES, 6);
		s_data.screen_vao->un_bind();

		s_data.fxaa_shader->un_bind();
		s_data.fxaa_fbo->un_bind();
	}

	void scene_renderer::final_pass()
	{
		RT_PROFILE;
		s_data.final_fbo->bind();
		renderer::clear_screen();
		s_data.final_shader->bind();
		renderer::bind_texture(0, s_data.fxaa_fbo->get_attachment_id(0)); // FXAA Pass

		// Render screen quad
		s_data.screen_vao->bind();
		renderer::submit_elements(GL_TRIANGLES, 6);
		s_data.screen_vao->un_bind();

		s_data.final_shader->un_bind();
		s_data.final_fbo->un_bind();
	}

	void scene_renderer::on_window_resize(const glm::ivec2& window_size)
	{
		s_data.geometry_fbo->resize(window_size);
		s_data.lighting_fbo->resize(window_size);
		s_data.fxaa_fbo->resize(window_size);
		s_data.final_fbo->resize(window_size);
		s_data.bloom_fbo->resize(window_size, false);
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

		s_data.screen_vao = std::make_shared<vertex_array_object>();
		std::shared_ptr<vertex_buffer_object> vertices_vbo = std::make_shared<
			vertex_buffer_object>(vertex_buffer_object_target::arrays);

		std::shared_ptr<vertex_buffer_object> index_buffer = std::make_shared<
			vertex_buffer_object>(vertex_buffer_object_target::elements, indices.size());

		s_data.screen_vao->bind();
		vertices_vbo->bind();
		vertices_vbo->set_data(vertex_buffer_object_usage::static_draw, vertex_buffer_size,
							   vertices.data());

		index_buffer->bind();
		index_buffer->set_data(vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

		std::initializer_list<vertex_buffer_layout_entry>
			layout_elements = {
				{"a_pos", vertex_buffer_entry_type::vec_float3, false},
				{"a_tex_coord", vertex_buffer_entry_type::vec_float2, false},
			};

		std::shared_ptr<vertex_buffer_layout_descriptor>
			vertices_vbo_layout_descriptor = std::make_shared<vertex_buffer_layout_descriptor>(
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

	void scene_renderer::setup_bloom_pass()
	{
		RT_PROFILE;
		glm::vec2 viewport_size = renderer::get_viewport_size();
		glm::ivec2 viewport_size_int = renderer::get_viewport_size();

		s_data.bloom_filter_radius = 0.005f;
		s_data.bloom_sample_count = 6;
		s_data.bloom_mips.resize(s_data.bloom_sample_count);

		// Create the bloom fbo
		s_data.bloom_fbo = std::make_shared<frame_buffer>(renderer::get_viewport_size().x, renderer::get_viewport_size().y);

		// Setup bloom fbo
		for (int i = 0; i < s_data.bloom_sample_count; i++)
		{
			bloom_mip_data bloom_mip;
			viewport_size *= 0.5f;
			viewport_size_int /= 2;

			bloom_mip.size = viewport_size_int;

			bloom_mip.texture =
				std::make_shared<texture>(std::format("bloom_mip_{}", i),
										  texture_data(bloom_mip.size.x, bloom_mip.size.y, 3, 1,
													   texture_internal_format::r11g11b10,
													   texture_type::none, nullptr));

			s_data.bloom_mips[i] = bloom_mip;
		}

		// Attach first bloom mip texture to rbo.
		s_data.bloom_fbo->attach_texture(s_data.bloom_mips[0].texture, GL_FRAMEBUFFER, render_buffer_attachment_type::color, GL_TEXTURE_2D, true, 0);
		s_data.bloom_fbo->initialize();

		// Load shaders
		s_data.bloom_down_sample_shader = shader_loader::load_shader_from_file(
			"resources/shaders/bloom/bloom_downsample.rrs");
		s_data.bloom_up_sample_shader = shader_loader::load_shader_from_file(
			"resources/shaders/bloom/bloom_upsample.rrs");
		s_data.bloom_composition_shader = shader_loader::load_shader_from_file(
			"resources/shaders/bloom/bloom_composition.rrs");
	}

	void scene_renderer::setup_fxaa_pass()
	{
		RT_PROFILE;
		glm::ivec2 viewport_size = renderer::get_viewport_size();
		std::vector<frame_buffer_attachment> fxaa_fbo_attachments = {
			{texture_internal_format::rgba16f,
			 texture_filtering::linear,
			 texture_wrapping::clamp_to_edge, viewport_size},
		};
		s_data.fxaa_fbo = std::make_shared<frame_buffer>(
			fxaa_fbo_attachments, viewport_size.x, viewport_size.y);
		s_data.fxaa_fbo->initialize();
		s_data.fxaa_shader = shader_loader::load_shader_from_file("resources/shaders/antialiasing/fxaa.rrs");
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