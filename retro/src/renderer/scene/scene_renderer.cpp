#include "rtpch.h"
#include "scene_renderer.h"

#include "renderer/shaders/shader_loader.h"

namespace retro::renderer
{
	scene_renderer_data scene_renderer::s_data = {};

	void scene_renderer::initialize()
	{
		glm::ivec2 viewport_size = renderer::get_viewport_size();
		/* Geometry Pass */

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
			texture_wrapping::clamp_to_edge, viewport_size}
		};
		frame_buffer_attachment geometry_fbo_depth_attachment = {
			texture_internal_format::depth_component32f,
			texture_filtering::linear,
			texture_wrapping::clamp_to_edge, viewport_size
		};
		s_data.geometry_fbo = std::make_shared<frame_buffer>(
			geometry_fbo_attachments, viewport_size.x, viewport_size.y, geometry_fbo_depth_attachment);
		s_data.geometry_fbo->initialize();

		s_data.geometry_shader = shader_loader::load_shader_from_file("resources/shaders/geometry.rrs");
	}

	void scene_renderer::begin_render(const std::shared_ptr<camera::camera> &camera)
	{
		s_data.camera = camera;
	}

	void scene_renderer::end_render()
	{
	}

	void scene_renderer::geometry_pass()
	{
	}

}