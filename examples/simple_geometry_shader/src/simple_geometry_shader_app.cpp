#include "simple_geometry_shader_app.h"

#include <glm/ext/matrix_transform.hpp>

simple_geometry_shader_app::simple_geometry_shader_app() : application("./")
{
    load_shaders();
    setup_model();
    setup_camera();
}

simple_geometry_shader_app::~simple_geometry_shader_app()
{
}

void simple_geometry_shader_app::on_update()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});

    // 1. Draw normally using model shader
    m_shader->bind();
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();

    // 2. Draw normals
    m_normals_shader->bind();
    m_normals_shader->set_mat4("u_transform", model);
    m_normals_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_normals_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::submit_model(m_model);
    m_normals_shader->un_bind();
}

void simple_geometry_shader_app::load_shaders()
{
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "../resources/shaders/model-colored.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/display-normals.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_normals_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

void simple_geometry_shader_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/monkey.obj");
}

void simple_geometry_shader_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 5.0f});
}

retro::core::application *retro::core::create_application()
{
    return new simple_geometry_shader_app();
}
