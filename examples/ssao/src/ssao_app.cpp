#include "rtpch.h"
#include "ssao_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <random>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

ssao_app::ssao_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_camera();
    setup_fbo();
    setup_screen_quad();
    setup_ssao();

    glEnable(GL_DEPTH_TEST);
}

ssao_app::~ssao_app()
{
}

void ssao_app::on_update()
{
    // 1. Render to geometry buffer
    m_geometry_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_geometry_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
    m_geometry_shader->set_mat4("u_transform", model);
    m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    retro::renderer::renderer::bind_texture(0, m_albedo_texture->get_handle_id());
    retro::renderer::renderer::bind_texture(1, m_normal_texture->get_handle_id());
    retro::renderer::renderer::submit_model(m_model);
    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();

    // 2. Render quad using geometry buffer

    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssao_fbo);
        glClear(GL_COLOR_BUFFER_BIT);
        m_ssao_shader->bind();
        m_ssao_shader->set_int("u_kernel_size", m_ssao_kernel.size());
        m_ssao_shader->set_float("u_ssao_radius", m_ssao_radius);
        m_ssao_shader->set_float("u_ssao_bias", m_ssao_bias);
        glm::vec2 noise_scale = glm::vec2(retro::renderer::renderer::get_viewport_size().x / m_ssao_noise_size, retro::renderer::renderer::get_viewport_size().y / m_ssao_noise_size);
        m_ssao_shader->set_vec_float2("u_noise_size", noise_scale);
        m_ssao_shader->set_mat4("u_view", m_camera->get_view_matrix());
        m_ssao_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
        for (int i = 0; i < 64; i++)
        {
            m_ssao_shader->set_vec_float3("u_samples[" + std::to_string(i) + "]", m_ssao_kernel[i]);
        }
        retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
        retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(2)); // Normal
        retro::renderer::renderer::bind_texture(2, m_ssao_noise_texture);                 // SSAO Noise
        m_quad_vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        m_ssao_shader->un_bind();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    {
        m_ssao_color_fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        m_ssao_shader->bind();
        m_ssao_shader->set_int("u_kernel_size", m_ssao_kernel.size());
        m_ssao_shader->set_float("u_ssao_radius", m_ssao_radius);
        m_ssao_shader->set_float("u_ssao_bias", m_ssao_bias);
        glm::vec2 noise_scale = glm::vec2(retro::renderer::renderer::get_viewport_size().x / m_ssao_noise_size, retro::renderer::renderer::get_viewport_size().y / m_ssao_noise_size);
        m_ssao_shader->set_vec_float2("u_noise_size", noise_scale);
        m_ssao_shader->set_mat4("u_view", m_camera->get_view_matrix());
        m_ssao_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
        for (int i = 0; i < 64; i++)
        {
            m_ssao_shader->set_vec_float3("u_samples[" + std::to_string(i) + "]", m_ssao_kernel[i]);
        }
        retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
        retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(2)); // Normal
        retro::renderer::renderer::bind_texture(2, m_ssao_noise_texture);                 // SSAO Noise
        m_quad_vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        m_ssao_shader->un_bind();
        m_ssao_color_fbo->un_bind();
    }

    // 3. Render lighting result
    retro::renderer::renderer::clear_screen();
    m_lighting_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0));   // Position
    retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1));   // Albedo
    retro::renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2));   // Normal
    retro::renderer::renderer::bind_texture(3, m_ssao_color_fbo->get_attachment_id(0)); // SSAO
    m_lighting_shader->set_vec_float3("p_light.position", m_light_pos);
    m_lighting_shader->set_vec_float3("p_light.color", m_light_color);
    m_lighting_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    m_lighting_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_lighting_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_quad_vao->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_lighting_shader->un_bind();

    retro::ui::interface::begin_frame();
    ImGui::Begin("SSAO");
    ImGui::Image((void *)(intptr_t)m_ssao_color_fbo->get_attachment_id(0), {400, 400}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Image((void *)(intptr_t)m_ssao_color_buffer, {400, 400}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Image((void *)(intptr_t)m_geometry_fbo->get_attachment_id(1), {400, 400}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    ImGui::Begin("Camera");
    glm::vec3 cam_pos = m_camera->get_position();
    if (ImGui::SliderFloat3("Camera Pos", glm::value_ptr(cam_pos), -10.0f, 10.0f))
    {
        m_camera->set_position(cam_pos);
    }
    ImGui::End();
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

    ImGui::End();
    retro::ui::interface::end_frame();
}

void ssao_app::load_shaders()
{
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/ssao.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_ssao_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/geometry.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_geometry_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/lighting.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_lighting_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

void ssao_app::load_texture()
{
    m_albedo_texture = std::make_shared<retro::renderer::texture>("../resources/models/tv/tv-albedo.png");
    m_normal_texture = std::make_shared<retro::renderer::texture>("../resources/models/tv/tv-normal.png");
}

void ssao_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");
}

void ssao_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void ssao_app::setup_fbo()
{
    // 1. Create geometry fbo.
    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                GL_RGBA,
                GL_RGB16F,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Albedo
            {
                GL_RGBA,
                GL_RGB16F,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Normals
            {
                GL_RGBA,
                GL_RGB16F,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            }};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            GL_RGBA,
            GL_RGBA16F,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(viewport_size.x, viewport_size.y, true, attachments, depth_attachment);
    }

    // 2. Setup ssao color buffer
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  SSAO Color
            {
                GL_RED,
                GL_RED,
                retro::renderer::texture_filtering::nearest,
                retro::renderer::texture_wrapping::none,
            }};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            GL_RGBA,
            GL_RGBA16F,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_ssao_color_fbo = std::make_shared<retro::renderer::frame_buffer>(viewport_size.x, viewport_size.y, false, attachments, depth_attachment);
    }
    // 3 Setup ssao blur buffer
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  SSAO Blur
            {
                GL_RED,
                GL_RED,
                retro::renderer::texture_filtering::nearest,
                retro::renderer::texture_wrapping::none,
            }};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            GL_RGBA,
            GL_RGBA16F,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_ssao_blur_fbo = std::make_shared<retro::renderer::frame_buffer>(viewport_size.x, viewport_size.y, false, attachments, depth_attachment);
    }
}

void ssao_app::setup_screen_quad()
{
    const std::vector<float> quad_vertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    const std::vector<uint32_t> indices = {
        0, 3, 1, // first triangle
        1, 3, 2, // second triangle
    };

    size_t vertex_buffer_size = quad_vertices.size() * sizeof(&quad_vertices[0]);
    size_t index_buffer_size = indices.size() * sizeof(&indices[0]);

    m_quad_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

    m_quad_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size, quad_vertices.data());

    index_buffer->bind();
    index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
        };

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
        vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

    m_quad_vao->add_vertex_buffer(vertices_vbo);
    m_quad_vao->set_index_buffer(index_buffer);
    m_quad_vao->un_bind();
}

void ssao_app::setup_ssao()
{
    m_ssao_radius = 0.5f;
    m_ssao_bias = 0.025f;
    m_ssao_noise_size = 4.0f;
    // 1. Setup ssao kernel
    std::uniform_real_distribution<float> random_floats(0.0, 1.0);
    std::default_random_engine generator;

    for (int i = 0; i < 64; i++)
    {
        glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator));

        sample = glm::normalize(sample);
        sample *= random_floats(generator);

        float scale = (float)i / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;

        m_ssao_kernel.push_back(sample);
    }

    // 2. Setup ssao noise
    std::vector<glm::vec3> ssao_noise;

    for (int i = 0; i < 16; i++)
    {
        glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0);
        ssao_noise.push_back(noise);
    }

    // Create noise texture
    glGenTextures(1, &m_ssao_noise_texture);
    glBindTexture(GL_TEXTURE_2D, m_ssao_noise_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssao_noise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    // 3. Setup ssao buffer
    glGenFramebuffers(1, &m_ssao_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssao_fbo);
    // SSAO color buffer
    glGenTextures(1, &m_ssao_color_buffer);
    glBindTexture(GL_TEXTURE_2D, m_ssao_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, viewport_size.x, viewport_size.y, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssao_color_buffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ssao_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::window_resize_event>([this](auto &&...args) -> decltype(auto)
                                                            { return ssao_app::on_resize_ssao(std::forward<decltype(args)>(args)...); });
}

bool ssao_app::on_resize_ssao(retro::events::window_resize_event &resize_event)
{
    // glBindTexture(GL_TEXTURE_2D, m_ssao_color_buffer);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resize_event.get_size().x, resize_event.get_size().y, 0, GL_RED, GL_FLOAT, nullptr);
    setup_ssao();
    RT_TRACE("SSAO Resize {0}, {0}", resize_event.get_size().x, resize_event.get_size().y);
    return false;
}

retro::core::application *retro::core::create_application()
{
    return new ssao_app();
}
