#include "simple_triangle_textured_app.h"

#include <glm/ext/matrix_transform.hpp>

simple_triangle_textured_app::simple_triangle_textured_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_triangle();
}

simple_triangle_textured_app::~simple_triangle_textured_app()
{
}

void simple_triangle_textured_app::on_update()
{
    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(glm::abs(glm::sin(static_cast<float>(glfwGetTime())))));
    m_shader->set_mat4("u_transform", model);
    retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    retro::renderer::renderer::submit_vao(m_triangle_vao, 3);
    m_shader->un_bind();
}

void simple_triangle_textured_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "../resources/shaders/screen-textured.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void simple_triangle_textured_app::load_texture()
{
    m_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/textures/texture.png");
}

void simple_triangle_textured_app::setup_triangle()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f};
    size_t size = vertices.size() * sizeof(&vertices[0]);

    m_triangle_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_triangle_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> triangle_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    triangle_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> triangle_vbo_vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    triangle_vbo->set_layout_descriptor(triangle_vbo_vbo_layout_descriptor);

    m_triangle_vao->add_vertex_buffer(triangle_vbo);
}

retro::core::application *retro::core::create_application()
{
    return new simple_triangle_textured_app();
}
