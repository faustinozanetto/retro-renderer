#include "simple_model_loading_app.h"

#include <glm/ext/matrix_transform.hpp>

simple_model_loading_app::simple_model_loading_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
}

simple_model_loading_app::~simple_model_loading_app()
{
}

void simple_model_loading_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {1, 1, 0});
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_shader->set_vec_float3("u_color", {0.35f, 0.75f, 0.25f});
    retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    retro::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();
}

void simple_model_loading_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/model-colored.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void simple_model_loading_app::load_texture()
{
    m_texture = std::make_shared<retro::renderer::texture>("resources/textures/texture.png");
}

void simple_model_loading_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("resources/models/monkey.obj");
}

void simple_model_loading_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 5.0f});
}

retro::core::application *retro::core::create_application()
{
    return new simple_model_loading_app();
}
