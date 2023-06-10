#include "material_preview.h"
#include "glm/ext/matrix_transform.inl"
#include "../material_editor_app.h"

namespace retro::material_editor
{
    material_preview::material_preview()
    {
        setup_shaders();
        setup_fbo();
        setup_camera();
        setup_model();
    }

    material_preview::~material_preview()
    {
    }

    int material_preview::get_final_render_target()
    {
        return m_geometry_fbo->get_attachment_id(1);
    }

    void material_preview::setup_fbo()
    {
        glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();

        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Albedo
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Normals
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Roughness Metallic AO
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Emissive
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size}};

        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_internal_format::depth_component32f,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge, viewport_size};

        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
        m_geometry_fbo->initialize();
    }

    void material_preview::setup_shaders()
    {
        m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
            "resources/shaders/geometry.rrs");
    }

    void material_preview::setup_camera()
    {
		m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
		m_camera->set_position({ 0.0f, 0.5f, 12.0f });
    }

    void material_preview::setup_model()
    {
        m_preview_model = renderer::model_loader::load_model_from_file("resources/models/monkey.obj");
    }

    void material_preview::render_preview()
    {
        const auto& app = dynamic_cast<material_editor_app*>(&core::application::get());

		// 1. Render to geometry fbo
		renderer::renderer::set_state(renderer::renderer_state::blend, false);
		renderer::renderer::set_state(renderer::renderer_state::depth, true);
		m_geometry_fbo->bind();
		renderer::renderer::clear_screen();
		m_geometry_shader->bind();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), { 0, 0.05, 0 });
		m_geometry_shader->set_mat4("u_transform", model);
		m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
		m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
		app->get_current_material()->bind(m_geometry_shader);
		renderer::renderer::submit_model(m_preview_model);
		m_geometry_shader->un_bind();
		m_geometry_fbo->un_bind();
		renderer::renderer::set_state(renderer::renderer_state::depth, false);
    }
}