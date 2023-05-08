﻿#include "parallax_mapping_app.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

parallax_mapping_app::parallax_mapping_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
    setup_light();

    m_parallax_scale = 0.12f;
}

parallax_mapping_app::~parallax_mapping_app()
{
}

void parallax_mapping_app::on_update()
{

    m_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_object_pos);
    model = glm::scale(model, glm::vec3(5.0f));
    const glm::mat4 rotation = glm::toMat4(glm::quat(m_object_rot));
    model *= rotation;
    //   model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    // model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
    m_shader->set_mat4("u_transform", model);
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    m_shader->set_vec_float3("p_light.position", m_light_pos);
    m_shader->set_vec_float3("p_light.color", m_light_color);
    m_shader->set_float("u_parallax_scale", m_parallax_scale);
    retro::renderer::renderer::bind_texture(0, m_albedo_texture->get_handle_id());
    retro::renderer::renderer::bind_texture(1, m_normal_texture->get_handle_id());
    retro::renderer::renderer::bind_texture(2, m_height_texture->get_handle_id());
    retro::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();

    retro::ui::interface::begin_frame();
    ImGui::Begin("Light");
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
    ImGui::SliderFloat("Parallax Scale", &m_parallax_scale, 0.01f, 2.0f);
    ImGui::End();
    ImGui::Begin("Object");
    glm::vec3 obj_pos = m_object_pos;
    if (ImGui::SliderFloat3("Position", glm::value_ptr(obj_pos), -2.0f, 2.0f))
    {
        m_object_pos = obj_pos;
    }
    glm::vec3 obj_rot = m_object_rot;
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(obj_rot), -10.0f, 10.0f))
    {
        m_object_rot = obj_rot;
    }
    ImGui::End();
    retro::ui::interface::end_frame();
}

void parallax_mapping_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
        "resources/shaders/parallax-mapping.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

void parallax_mapping_app::load_texture()
{
    m_albedo_texture = std::make_shared<retro::renderer::texture>("../resources/textures/brick-wall/beaten-down-brick_albedo.png");
    m_normal_texture = std::make_shared<retro::renderer::texture>("../resources/textures/brick-wall/beaten-down-brick_normal-ogl.png");
    m_height_texture = std::make_shared<retro::renderer::texture>("../resources/textures/brick-wall/beaten-down-brick_height.png");
}

void parallax_mapping_app::setup_model()
{
    m_object_pos = glm::vec3(0);
    m_object_rot = glm::vec3(0);
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/plane.obj");
}

void parallax_mapping_app::setup_light()
{
    m_light_pos = glm::vec3(0.0f, 0.0f, 5.0f);
    m_light_color = glm::vec3(0.85f);
    m_point_light = std::make_shared<retro::renderer::point_light>(m_light_pos, m_light_color);
}

void parallax_mapping_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

retro::core::application *retro::core::create_application()
{
    return new parallax_mapping_app();
}