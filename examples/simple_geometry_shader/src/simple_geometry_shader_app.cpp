#include "simple_geometry_shader_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>

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
    m_shader->set_vec_float3("u_color", {0.15f, 0.65f, 0.45f});
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
    m_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/model-colored.rrs");

    m_normals_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/display-normals.rrs");
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

void simple_geometry_shader_app::on_handle_event(retro::events::base_event& event)
{
}

bool simple_geometry_shader_app::on_window_resize(retro::events::window_resize_event& resize_event)
{
    return application::on_window_resize(resize_event);
}

retro::core::application* retro::core::create_application()
{
    return new simple_geometry_shader_app();
}
