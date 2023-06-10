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
        setup_screen();
        setup_lighting();
        setup_scene();
    }

    material_preview::~material_preview()
    {
    }

    int material_preview::get_final_render_target()
    {
        return m_final_fbo->get_attachment_id(0);
    }

    void material_preview::setup_fbo()
    {
        glm::ivec2 viewport_size = renderer::renderer::get_viewport_size();

        std::vector<renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                renderer::texture_internal_format::rgba16f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Albedo
            {
                renderer::texture_internal_format::rgba16f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Normals
            {
                renderer::texture_internal_format::rgba16f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Roughness Metallic AO
            {
                renderer::texture_internal_format::rgba16f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Emissive
            {
                renderer::texture_internal_format::rgba16f,
                renderer::texture_filtering::linear,
                renderer::texture_wrapping::clamp_to_edge, viewport_size}};

        renderer::frame_buffer_attachment depth_attachment = {
            renderer::texture_internal_format::depth_component32f,
            renderer::texture_filtering::linear,
            renderer::texture_wrapping::clamp_to_edge, viewport_size};

        m_geometry_fbo = std::make_shared<renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
        m_geometry_fbo->initialize();
    }

    void material_preview::setup_shaders()
    {
        m_geometry_shader = renderer::shader_loader::load_shader_from_file(
            "resources/shaders/geometry.rrs");
    }

    void material_preview::setup_camera()
    {
        m_camera = std::make_shared<camera::camera>(camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
        m_camera->set_position({0.0f, 0.5f, 12.0f});
    }

    void material_preview::setup_model()
    {
        m_preview_model = renderer::model_loader::load_model_from_file("resources/models/radio/radio.obj");
    }

    void material_preview::setup_scene()
    {
        const auto &app = dynamic_cast<material_editor_app *>(&core::application::get());

        m_preview_scene = std::make_shared<scene::scene>("preview scene");
        scene::scene_manager::get().set_active_scene(m_preview_scene);

        // Setup actor
        m_preview_actor = m_preview_scene->create_actor("preview actor");
        m_preview_actor->add_component<scene::transform_component>();
        m_preview_actor->add_component<scene::model_renderer_component>(m_preview_model);
        m_preview_actor->add_component<scene::material_renderer_component>(app->get_current_material());
    }

    void material_preview::setup_screen()
    {
		std::vector<float> quad_vertices = {
	   1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
	   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
	   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
	   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
		};

		std::vector<uint32_t> indices = {
			0, 3, 1, // first triangle
			1, 3, 2, // second triangle
		};

		size_t vertex_buffer_size = quad_vertices.size() * sizeof(&quad_vertices[0]);
		size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

		m_screen_vao = std::make_shared<renderer::vertex_array_object>();
		std::shared_ptr<renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
			renderer::vertex_buffer_object>(renderer::vertex_buffer_object_target::arrays);

		std::shared_ptr<renderer::vertex_buffer_object> index_buffer = std::make_shared<
			renderer::vertex_buffer_object>(renderer::vertex_buffer_object_target::elements, indices.size());

		m_screen_vao->bind();
		vertices_vbo->bind();
		vertices_vbo->set_data(renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
			quad_vertices.data());

		index_buffer->bind();
		index_buffer->set_data(renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

		std::initializer_list<renderer::vertex_buffer_layout_entry>
			layout_elements = {
				{"a_pos", renderer::vertex_buffer_entry_type::vec_float3, false},
				{"a_tex_coord", renderer::vertex_buffer_entry_type::vec_float2, false},
		};

		std::shared_ptr<renderer::vertex_buffer_layout_descriptor>
			vertices_vbo_layout_descriptor = std::make_shared<renderer::vertex_buffer_layout_descriptor>(
				layout_elements);
		vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

		m_screen_vao->add_vertex_buffer(vertices_vbo);
		m_screen_vao->set_index_buffer(index_buffer);
		m_screen_vao->un_bind();

		m_screen_shader = renderer::shader_loader::load_shader_from_file(
			"resources/shaders/screen.rrs");
    }

    void material_preview::setup_lighting()
    {
        // Create lighting fbo
        glm::ivec2 viewport_size = renderer::renderer::get_viewport_size();
        {
            std::vector<renderer::frame_buffer_attachment> attachments = {
                //  Position
                {
                    renderer::texture_internal_format::rgba16f,
                    renderer::texture_filtering::linear,
                    renderer::texture_wrapping::clamp_to_edge, viewport_size},
            };
            m_lighting_fbo = std::make_shared<renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y);
            m_lighting_fbo->initialize();
        }
		{
			std::vector<renderer::frame_buffer_attachment> attachments = {
				//  Position
				{
					renderer::texture_internal_format::rgba16f,
					renderer::texture_filtering::linear,
					renderer::texture_wrapping::clamp_to_edge, viewport_size},
			};
            m_final_fbo = std::make_shared<renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y);
			m_final_fbo->initialize();
		}

		m_lighting_shader = renderer::shader_loader::load_shader_from_file(
			"resources/shaders/pbr.rrs");

		m_final_shader = renderer::shader_loader::load_shader_from_file(
			"resources/shaders/tonemapping.rrs");
    }

    void material_preview::on_material_selected(const std::shared_ptr<renderer::material> &material)
    {
        m_preview_actor->get_component<scene::material_renderer_component>().set_material(material);
    }

    void material_preview::render_preview()
    {
        const auto &app = dynamic_cast<material_editor_app *>(&core::application::get());

        // 1. Render to geometry fbo
        renderer::renderer::set_state(renderer::renderer_state::blend, false);
        renderer::renderer::set_state(renderer::renderer_state::depth, true);
        m_geometry_fbo->bind();
        renderer::renderer::clear_screen();

        auto &transform_component = m_preview_actor->get_component<scene::transform_component>();
        auto &model_renderer_component = m_preview_actor->get_component<scene::model_renderer_component>();
        auto &material_renderer_component = m_preview_actor->get_component<scene::material_renderer_component>();

        m_geometry_shader->bind();
        m_geometry_shader->set_mat4("u_transform", transform_component.get_transform());
        m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
        m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());

        material_renderer_component.get_material()->bind(m_geometry_shader);
        renderer::renderer::submit_model(model_renderer_component.get_model());

        m_geometry_shader->un_bind();
        m_geometry_fbo->un_bind();
        renderer::renderer::set_state(renderer::renderer_state::depth, false);

		// 2. Render geometry result using pbr to screen
		m_lighting_fbo->bind();
		renderer::renderer::clear_screen();
		m_lighting_shader->bind();
        m_lighting_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
        m_lighting_shader->set_vec_float3("u_directional_light.position", { 0.0f, 0.0f, 0.0f });
        m_lighting_shader->set_vec_float3("u_directional_light.direction", { 1.8f, -5.5f, -3.6f });
        m_lighting_shader->set_vec_float3("u_directional_light.color", { 5.0f, 5.0f, 5.0f });
		renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
		renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1)); // Albedo
		renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2)); // Normal
		renderer::renderer::bind_texture(3, m_geometry_fbo->get_attachment_id(3)); // Roughness - Metallic - AO
		renderer::renderer::bind_texture(4, m_geometry_fbo->get_attachment_id(4)); // Emissive
		m_screen_vao->bind();
		renderer::renderer::submit_elements(GL_TRIANGLES, 6);
		m_screen_vao->un_bind();
        m_lighting_shader->un_bind();
		m_lighting_fbo->un_bind();

        // 3. Render final pass using the tonemapping shader
        m_final_fbo->bind();
        renderer::renderer::clear_screen();
        m_final_shader->bind();
        renderer::renderer::bind_texture(0, m_lighting_fbo->get_attachment_id(0));
		m_screen_vao->bind();
		renderer::renderer::submit_elements(GL_TRIANGLES, 6);
		m_screen_vao->un_bind();
        m_final_shader->un_bind();
        m_final_fbo->un_bind();
    }
}