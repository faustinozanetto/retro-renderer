#include "simple_fonts_app.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui.h>
#include <string>
#include <format>
#include <random>

simple_fonts_app::simple_fonts_app() : application("./")
{
    load_shaders();
    load_font();
    setup_triangle();

    m_test_text = std::make_shared<retro::renderer::text>("This is a simple test!", glm::vec2(250.0f, 250.0f));

    retro::renderer::renderer::set_vsync_enabled(false);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    const std::string& text_content= "Testing texts s 458 @WOW!";
    int column_height = 50;
    
    for (int i = 0; i < 400; i++) {
        int xIncrement = i / column_height * text_content.size() * 12 + 15.0f; // Calculate the X position increment
        int y = i % column_height * 25; // Calculate the Y position

        glm::vec2 position = { static_cast<float>(xIncrement), static_cast<float>(y) };

        
        auto text = std::make_shared<retro::renderer::text>(text_content, position, glm::vec2(0.5f), glm::vec3(dis(gen),dis(gen),dis(gen)));
        m_texts.push_back(text);
    }
}

simple_fonts_app::~simple_fonts_app()
{
}

void simple_fonts_app::on_update()
{
    retro::ui::interface::begin_frame();
    ImGuiIO &io = ImGui::GetIO();
    float framerate = io.Framerate;
    m_test_text->set_content(std::format("FPS: {}", framerate));
    retro::ui::interface::end_frame();

    m_test_text->set_posiiton({glm::sin(glfwGetTime()) * 300.0f + 250.0f, glm::cos(glfwGetTime()) * 300.0f + 250.0f});

    m_shader->bind();
    retro::renderer::renderer::submit_text(m_shader, m_font, m_test_text);
    for (auto&& text : m_texts) {
        retro::renderer::renderer::submit_text(m_shader, m_font, text);
    }
    m_shader->un_bind();
}

void simple_fonts_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/screen.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void simple_fonts_app::load_font()
{
    m_font = std::make_shared<retro::renderer::font>("resources/fonts/arial.ttf");
}

void simple_fonts_app::setup_triangle()
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
    return new simple_fonts_app();
}
