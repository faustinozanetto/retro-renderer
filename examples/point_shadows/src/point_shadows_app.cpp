#include "rtpch.h"
#include "point_shadows_app.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

point_shadows_app::point_shadows_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
    setup_light_cube();
    setup_shadow_map_frame_buffer();
    setup_light();
}

point_shadows_app::~point_shadows_app()
{
}

void point_shadows_app::on_update()
{
    // 0. create depth cubemap transformation matrices
    glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f), (float)m_shadow_map_size / (float)m_shadow_map_size, m_near_plane, m_far_plane);
    std::vector<glm::mat4> shadow_transforms;
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadow_transforms.push_back(shadow_projection * glm::lookAt(m_point_light->get_position(), m_point_light->get_position() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    // 1. render scene to the shadow cubemap
    retro::renderer::renderer::set_viewport_size({m_shadow_map_size, m_shadow_map_size});
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shadow_shader->bind();
    for (int i = 0; i < 6; ++i)
    {
        m_shadow_shader->set_mat4("u_shadow_transformations[" + std::to_string(i) + "]", shadow_transforms[i]);
    }
    m_shadow_shader->set_vec_float3("u_light_pos", m_point_light->get_position());
    m_shadow_shader->set_float("u_far_plane", m_far_plane);
    render_elements(m_shadow_shader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. render scene as usual
    retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader->bind();
    m_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_shader->set_vec_float3("p_light.position", m_point_light->get_position());
    m_shader->set_vec_float3("p_light.color", m_point_light->get_diffuse());
    m_shader->set_float("u_far_plane", m_far_plane);
    m_shader->set_float("u_shadow_bias", m_shadow_bias);
    m_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    retro::renderer::renderer::bind_texture(1, m_depth_cubemap);
    render_elements(m_shader);
    m_shader->un_bind();

   // m_point_light->get_position().z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
   // m_point_light->get_position().x = static_cast<float>(cos(glfwGetTime() * 0.5) * 3.0);

    retro::ui::interface::begin_frame();
    ImGui::Begin("Object");
    glm::vec3 obj_pos = m_object_pos;
    if (ImGui::SliderFloat3("Position", glm::value_ptr(obj_pos), -10.0f, 10.0f))
    {
        m_object_pos = obj_pos;
    }
    glm::vec3 obj_rot = m_object_rot;
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(obj_rot), -10.0f, 10.0f))
    {
        m_object_rot = obj_rot;
    }
    ImGui::End();

    ImGui::Begin("Light");
    glm::vec3 position = m_point_light->get_position();
    if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f))
    {
        m_point_light->set_position(position);
    }
    ImGui::SliderFloat("Bias", &m_shadow_bias, 0.01f, 5.0f);
    ImGui::SliderFloat("Near Plane", &m_near_plane, 0.01f, 10.0f);
    ImGui::SliderFloat("Far Plane", &m_far_plane, 0.1f, 100.0f);
    glm::vec3 color = m_point_light->get_diffuse();
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
    {
        m_point_light->set_diffuse(color);
    }
    ImGui::End();
    glm::vec3 cam_pos = m_camera->get_position();
    ImGui::Begin("Camera");
    if (ImGui::SliderFloat3("Camera Pos", glm::value_ptr(cam_pos), -10.0f, 10.0f))
    {
        m_camera->set_position(cam_pos);
    }
    ImGui::End();
    retro::ui::interface::end_frame();
}

void point_shadows_app::load_shaders()
{
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/lighting.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }

    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/point-shadows.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_shadow_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

void point_shadows_app::load_texture()
{
    m_texture = retro::renderer::texture_loader::load_texture_from_file("resources/textures/wood.jpg");
}

void point_shadows_app::setup_model()
{
    m_object_pos = glm::vec3(0);
    m_object_rot = glm::vec3(0);
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");
}

void point_shadows_app::setup_light()
{
    m_point_light = std::make_shared<retro::renderer::point_light>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.85f), glm::vec3(1.0f));
}

void point_shadows_app::setup_light_cube()
{
    std::vector<float>
        vertices = {
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-left
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,   // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,   // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,     // top-right
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,     // top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,    // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-left
            -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // top-right
            -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  // top-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // top-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // top-left
            1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,    // top-right
            1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // top-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // bottom-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,   // bottom-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,   // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // top-left
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // top-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        };
    size_t size = vertices.size() * sizeof(&vertices[0]);

    m_light_cube_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_light_cube_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> cube_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    cube_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
            {"a_normal", retro::renderer::vertex_buffer_entry_type::vec_float3, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> cube_vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    cube_vbo->set_layout_descriptor(cube_vbo_layout_descriptor);

    m_light_cube_vao->add_vertex_buffer(cube_vbo);
}

void point_shadows_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 55.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 4.5f});
}

void point_shadows_app::setup_shadow_map_frame_buffer()
{
    m_shadow_bias = 0.4f;
    m_near_plane = 0.25f;
    m_far_plane = 30.0f;
    m_shadow_map_size = 2048;
    glGenFramebuffers(1, &m_shadow_map_fbo);
    glGenTextures(1, &m_depth_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depth_cubemap);

    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadow_map_size, m_shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_cubemap, 0);
    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    // Disable reads from the color buffer
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    RT_ASSERT_MSG(status == GL_FRAMEBUFFER_COMPLETE, "An error ocurred while creating shadow fbo!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void point_shadows_app::render_elements(const std::shared_ptr<retro::renderer::shader> &shader)
{
    // Render outside box
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(7.0f));
        shader->set_mat4("u_transform", model);
        glDisable(GL_CULL_FACE);
        shader->set_int("u_reverse_normals", 1);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        shader->set_int("u_reverse_normals", 0);
        glEnable(GL_CULL_FACE);
    }
    // Render model
    {
        /*
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.10f));
        model = glm::translate(model, m_object_pos);
        const glm::mat4 rotation = glm::toMat4(glm::quat(m_object_rot));
        model *= rotation;
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
        retro::renderer::renderer::submit_model(m_model);
        */
        // cubes
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.75f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.75f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
    }
    // Render Light Cube
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_point_light->get_position());
        model = glm::scale(model, glm::vec3(0.1f));
        shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_vao(m_light_cube_vao, 36);
    }
}

retro::core::application *retro::core::create_application()
{
    return new point_shadows_app();
}
