#include "bloom_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>

bloom_app::bloom_app() : application("./")
{
    load_shaders();
    setup_model();
    setup_camera();
    setup_frame_buffers();
    setup_screen_quad();
}

bloom_app::~bloom_app()
{
}

void bloom_app::on_update()
{
    glDisable(GL_BLEND);
    // 1. Render to geometry fbo
    m_geometry_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_geometry_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(2.0f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
    m_geometry_shader->set_mat4("u_transform", model);
    m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_material->bind(m_geometry_shader);
    retro::renderer::renderer::submit_model(m_model);
    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();

    // 2. Render geometry result using pbr to screen
    // m_lighting_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_pbr_shader->bind();
    m_pbr_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    m_pbr_shader->set_vec_float3("u_directional_light.position", {0.0f, 0.0f, 0.0f});
    m_pbr_shader->set_vec_float3("u_directional_light.direction", {1.8f, -5.5f, -3.6f});
    m_pbr_shader->set_vec_float3("u_directional_light.color", {10.0f, 10.0f, 10.0f});
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
    retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1)); // Albedo
    retro::renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2)); // Normal
    retro::renderer::renderer::bind_texture(3, m_geometry_fbo->get_attachment_id(3)); // Roughness - Metallic - AO
    retro::renderer::renderer::bind_texture(4, m_geometry_fbo->get_attachment_id(4)); // Emissive
    m_screen_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_screen_vao->un_bind();
    m_pbr_shader->un_bind();
    // m_lighting_fbo->un_bind();
}

void bloom_app::load_shaders()
{
    m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/geometry.rrs");
    m_pbr_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/pbr.rrs");
    m_screen_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/screen.rrs");
}

void bloom_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/radio/radio.obj");
    m_material = retro::renderer::material_loader::load_material_from_file("../resources/materials/radio.rrm");
}

void bloom_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void bloom_app::setup_screen_quad()
{
    const std::vector<float> quad_vertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    const std::vector<uint32_t> indices = {
        0, 3, 1, // first triangle
        1, 3, 2, // second triangle
    };

    size_t vertex_buffer_size = quad_vertices.size() * sizeof(&quad_vertices[0]);
    size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

    m_screen_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

    m_screen_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
                           quad_vertices.data());

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

    m_screen_vao->add_vertex_buffer(vertices_vbo);
    m_screen_vao->set_index_buffer(index_buffer);
    m_screen_vao->un_bind();
}

void bloom_app::setup_frame_buffers()
{
    // 1. Create geometry fbo.
    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Albedo
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Normals
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Roughness Metallic AO
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Emissive
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            }};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_format::depth_component32f,
            retro::renderer::texture_internal_format::rgba,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
    }
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
        };
        m_lighting_fbo = std::make_shared<retro::renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y);
    }
}

void bloom_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(bloom_app::on_window_resize));
}

bool bloom_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size());
    m_lighting_fbo->resize(resize_event.get_size());
    return application::on_window_resize(resize_event);
}

retro::core::application *retro::core::create_application()
{
    return new bloom_app();
}
