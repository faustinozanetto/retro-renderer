﻿#include "simple_triangle_app.h"

#include <glm/ext/matrix_transform.hpp>

simple_triangle_app::simple_triangle_app() : application("./")
{
    load_shaders();
    setup_triangle();
}

simple_triangle_app::~simple_triangle_app()
{
}

void simple_triangle_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(glm::abs(glm::sin(static_cast<float>(glfwGetTime())))));
    m_shader->set_mat4("u_transform", model);
    m_shader->set_vec_float3("u_color", {0.15f, 0.45f, 0.75f});
    m_triangle_vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_shader->un_bind();
}

void simple_triangle_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/screen.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void simple_triangle_app::setup_triangle()
{
    m_triangle_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> triangle_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    m_triangle_vao->bind();

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f};
    size_t size = vertices.size() * sizeof(&vertices[0]);

    triangle_vbo->bind();
    triangle_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());
    triangle_vbo->set_attribute(0, 3, GL_FLOAT, 5 * sizeof(float), nullptr);
    triangle_vbo->set_attribute(1, 2, GL_FLOAT, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    triangle_vbo->un_bind();
    m_triangle_vao->un_bind();
}

retro::core::application *retro::core::create_application()
{
    return new simple_triangle_app();
}
