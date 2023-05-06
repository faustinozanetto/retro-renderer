#include "simple_3d_app.h"

#include <glm/ext/matrix_transform.hpp>

simple_3d_app::simple_3d_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_cube();
    setup_camera();
}

simple_3d_app::~simple_3d_app()
{
}

void simple_3d_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {1, 1, 0});
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    retro::renderer::renderer::submit_vao(m_cube_vao, 36);
    m_shader->un_bind();
}

void simple_3d_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/screen_textured.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void simple_3d_app::load_texture()
{
    m_texture = std::make_shared<retro::renderer::texture>("resources/textures/texture.png");
}

void simple_3d_app::setup_cube()
{
    m_cube_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_cube_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> cube_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    size_t size = vertices.size() * sizeof(&vertices[0]);

    m_cube_vao->bind();
    cube_vbo->bind();
    cube_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());
    cube_vbo->set_attribute(0, 3, GL_FLOAT, 5 * sizeof(float), nullptr);
    cube_vbo->set_attribute(1, 2, GL_FLOAT, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    m_cube_vao->un_bind();
}

void simple_3d_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.0f, 3.0f});
}

retro::core::application *retro::core::create_application()
{
    return new simple_3d_app();
}
