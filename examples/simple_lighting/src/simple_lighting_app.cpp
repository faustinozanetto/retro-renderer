#include "simple_lighting_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

simple_lighting_app::simple_lighting_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
    setup_light_cube();
    setup_light();
}

simple_lighting_app::~simple_lighting_app()
{
}

void simple_lighting_app::on_update()
{
    {
        m_shader->bind();
        m_shader->set_mat4("u_view", m_camera->get_view_matrix());
        m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
        m_shader->set_vec_float3("p_light.position", m_light_pos);
        m_shader->set_vec_float3("p_light.color", m_light_color);

        m_shader->set_vec_float3("s_light.position", m_light_pos);
        m_shader->set_vec_float3("s_light.color", m_light_color);

        m_shader->set_vec_float3("s_light.direction", m_light_dir);
        m_shader->set_float("s_light.inner_angle", m_inner_angle);
        m_shader->set_float("s_light.outer_angle", m_outer_angle);
        m_shader->set_int("u_light_type", static_cast<int>(m_selected_light));
        m_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
        m_shader->set_mat4("u_transform", model);
        retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
        retro::renderer::renderer::submit_model(m_model);
        m_shader->un_bind();
    }
    {
        m_shader_light->bind();
        m_shader_light->set_mat4("u_view", m_camera->get_view_matrix());
        m_shader_light->set_mat4("u_projection", m_camera->get_projection_matrix());
        m_shader_light->set_vec_float3("u_color", m_light_color);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_light_pos);
        m_shader_light->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        m_shader_light->un_bind();
    }

    // m_light_pos.x = glm::sin((float)glfwGetTime()) * 8.5f;

    retro::ui::interface::begin_frame();
    ImGui::Begin("Light");
    if (ImGui::BeginCombo("Type", std::to_string(static_cast<int>(m_selected_light)).c_str()))
    {
        if (ImGui::Selectable("Point", m_selected_light == retro::renderer::light_type::point))
        {
            m_selected_light = retro::renderer::light_type::point;
        }
        if (ImGui::Selectable("Spot", m_selected_light == retro::renderer::light_type::spot))
        {
            m_selected_light = retro::renderer::light_type::spot;
        }
        ImGui::EndCombo();
    }

    glm::vec3 position = m_light_pos;
    if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f))
    {
        m_light_pos = position;
    }
    glm::vec3 color = m_light_color;
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
    {
        m_light_color = color;
    }

    if (m_selected_light == retro::renderer::light_type::spot)
    {
        glm::vec3 direction = m_light_dir;
        if (ImGui::SliderFloat3("Direction", glm::value_ptr(direction), -10.0f, 10.0f))
        {
            m_light_dir = direction;
        }
        float inner_angle = m_inner_angle;
        if (ImGui::SliderFloat("Inner Angle", &inner_angle, 0.0f, 3.0f))
        {
            m_inner_angle = inner_angle;
        }
        float outer_angle = m_outer_angle;
        if (ImGui::SliderFloat("Outer Angle", &outer_angle, 0.0f, 3.0f))
        {
            m_outer_angle = outer_angle;
        }
    }
    ImGui::End();
    retro::ui::interface::end_frame();
}

void simple_lighting_app::load_shaders()
{
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/model-textured-lighting.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }

    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "../resources/shaders/model-colored.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_shader_light = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

void simple_lighting_app::load_texture()
{
    m_texture = std::make_shared<retro::renderer::texture>("../resources/models/tv/tv-albedo.png");
}

void simple_lighting_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");
}

void simple_lighting_app::setup_light()
{
    m_selected_light = retro::renderer::light_type::point;
    m_light_pos = glm::vec3(0.0f, 0.0f, 5.0f);
    m_light_color = glm::vec3(0.85f);
    m_point_light = std::make_shared<retro::renderer::point_light>(m_light_pos, m_light_color);
    m_light_dir = m_camera->get_front();
    m_inner_angle = 1.45f;
    m_outer_angle = 0.5f;

    m_spot_light = std::make_shared<retro::renderer::spot_light>(m_light_pos, m_light_dir, m_inner_angle, m_outer_angle, m_light_color);
}

void simple_lighting_app::setup_light_cube()
{
    std::vector<float>
        vertices = {
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

    m_light_cube_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_light_cube_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> cube_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    cube_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> cube_vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    cube_vbo->set_layout_descriptor(cube_vbo_layout_descriptor);

    m_light_cube_vao->add_vertex_buffer(cube_vbo);
}

void simple_lighting_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

retro::core::application *retro::core::create_application()
{
    return new simple_lighting_app();
}
