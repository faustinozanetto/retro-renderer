#include "bloom_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>

bloom_app::bloom_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
}

bloom_app::~bloom_app()
{
}

void bloom_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    retro::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();
}

void bloom_app::load_shaders()
{
    m_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/model-textured.rrs");
}

void bloom_app::load_texture()
{
    m_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-albedo.png");
}

void bloom_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");
}

void bloom_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void bloom_app::on_handle_event(retro::events::base_event &event)
{
}

bool bloom_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    return application::on_window_resize(resize_event);
}

retro::core::application *retro::core::create_application()
{
    return new bloom_app();
}
