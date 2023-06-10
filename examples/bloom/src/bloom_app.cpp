#include "bloom_app.h"

#include <glm/ext/matrix_transform.hpp>
#include <core/entry_point.h>
#include <glm/gtc/type_ptr.hpp>

#include <renderer/materials/material.h>
#include <utils/files.h>
#include <logging/logger.h>

bloom_app::bloom_app() : application("./")
{
    load_shaders();
    setup_model();
    setup_camera();
    setup_frame_buffers();
    setup_screen_quad();
    m_filter_radius = 0.005f;
    m_bloom_sample_count = 6;
    m_model_pos = glm::vec3(0.0f);
    setup_bloom();
    m_final_render_target = m_lighting_fbo->get_attachment_id(0);

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../resources/fonts/arial.ttf", 16);
}

bloom_app::~bloom_app()
{
}

void bloom_app::on_update()
{
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::blend, false);
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::depth, true);
    // 1. Render to geometry fbo
    m_geometry_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_geometry_shader->bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.07f));
    model = glm::translate(model, m_model_pos);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 0.05, 0});
    m_geometry_shader->set_mat4("u_transform", model);
    m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_material->bind(m_geometry_shader);
    retro::renderer::renderer::submit_model(m_model);
    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::depth, false);

    // 2. Render geometry result using pbr to screen
    m_lighting_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_pbr_shader->bind();
    m_pbr_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    m_pbr_shader->set_vec_float3("u_directional_light.position", {0.0f, 0.0f, 0.0f});
    m_pbr_shader->set_vec_float3("u_directional_light.direction", {glm::sin(retro::core::time::get_time()), -5.5f, -3.6f});
    m_pbr_shader->set_vec_float3("u_directional_light.color", {10.0f, 10.0f, 10.0f});
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
    retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1)); // Albedo
    retro::renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2)); // Normal
    retro::renderer::renderer::bind_texture(3, m_geometry_fbo->get_attachment_id(3)); // Roughness - Metallic - AO
    retro::renderer::renderer::bind_texture(4, m_geometry_fbo->get_attachment_id(4)); // Emissive
    m_screen_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_screen_vao->un_bind();
    m_pbr_shader->un_bind();
    m_lighting_fbo->un_bind();

    // 3. Bloom downsample pass
    m_bloom_fbo->bind();
    m_bloom_downsample_shader->bind();
    // Bind pbr result texture
    retro::renderer::renderer::bind_texture(0, m_lighting_fbo->get_attachment_id(0));
    m_bloom_downsample_shader->set_vec_float2("u_source_res", retro::renderer::renderer::get_viewport_size());
    m_bloom_downsample_shader->set_int("mipLevel", 0);
    for (int i = 0; i < (int)m_bloom_mips.size(); i++)
    {
        bloom_mip_data &bloom_mip = m_bloom_mips[i];
        retro::renderer::renderer::set_viewport_size(bloom_mip.size);
        // Attach bloom mip textue to rbo.
        m_bloom_fbo->attach_texture(bloom_mip.texture, GL_FRAMEBUFFER, retro::renderer::render_buffer_attachment_type::color, GL_TEXTURE_2D, 0);

        // Draw screen quad
        m_screen_vao->bind();
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_screen_vao->un_bind();

        // Set current mip resolution as srcResolution for next iteration
        m_bloom_downsample_shader->set_vec_float2("u_source_res", bloom_mip.size);
        // Set current mip as texture input for next iteration
        retro::renderer::renderer::bind_texture(0, bloom_mip.texture->get_handle_id());
        // Disable Karis average for consequent downsamples
        if (i == 0)
        {
            m_bloom_downsample_shader->set_int("mipLevel", 1);
        }
    }
    m_bloom_downsample_shader->un_bind();

    // 4. Bloom upsample pass
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::blend, true);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    m_bloom_upsample_shader->bind();
    m_bloom_upsample_shader->set_float("u_filter_radius", m_filter_radius);
    for (int i = m_bloom_mips.size() - 1; i > 0; i--)
    {
        bloom_mip_data &bloom_mip = m_bloom_mips[i];
        bloom_mip_data &bloom_next_mip = m_bloom_mips[i - 1];
        // Bind mip texture
        retro::renderer::renderer::bind_texture(0, bloom_mip.texture->get_handle_id());
        retro::renderer::renderer::set_viewport_size(bloom_next_mip.size);
        // Attach bloom mip textue to rbo.
        m_bloom_fbo->attach_texture(bloom_next_mip.texture, GL_FRAMEBUFFER, retro::renderer::render_buffer_attachment_type::color, GL_TEXTURE_2D, 0);
        // Draw screen quad
        m_screen_vao->bind();
        retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
        m_screen_vao->un_bind();
    }
    retro::renderer::renderer::set_state(retro::renderer::renderer_state::blend, false);

    m_bloom_upsample_shader->un_bind();

    m_bloom_fbo->un_bind();

    // 5. Render final result to screen
    retro::renderer::renderer::clear_screen();
    retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
    m_final_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_final_render_target);
    retro::renderer::renderer::bind_texture(1, m_bloom_mips[0].texture->get_handle_id());
    m_screen_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_screen_vao->un_bind();
    m_final_shader->un_bind();

    // 6. ImGui Debug
    retro::ui::engine_ui::begin_frame();

    ImGui::Begin("Model");
    ImGui::SliderFloat3("Position", glm::value_ptr(m_model_pos), -50.0f, 50.0f);
    ImGui::End();

    ImGui::Begin("Bloom");
    ImGui::SliderFloat("Filter Radius", &m_filter_radius, 0.0001f, 0.001f);
    if (ImGui::SliderInt("Bloom Mip Count", &m_bloom_sample_count, 1, 10))
    {
        setup_bloom();
    }
    ImGui::Separator();

    ImGui::Text("Bloom Mips");
    int current_mip_index = 0;
    for (const bloom_mip_data &bloom_mip : m_bloom_mips)
    {
        bool packNodeOpen = ImGui::TreeNodeEx((void *)current_mip_index,
                                              ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed,
                                              "Bloom Mip Index: %d",
                                              current_mip_index);
        if (packNodeOpen)
        {
            ImGui::Text("Size: (%dx%d)", bloom_mip.size.x, bloom_mip.size.y);

            ImGui::Image((ImTextureID)(bloom_mip.texture->get_handle_id()), {256, 256}, {0, 1}, {1, 0});

            ImGui::TreePop();
        }
        current_mip_index++;
    }

    ImGui::End();
    retro::ui::engine_ui::end_frame();
}

void bloom_app::load_shaders()
{
    m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/geometry.rrs");
    m_pbr_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/pbr.rrs");
    m_screen_shader = retro::renderer::shader_loader::load_shader_from_file(
        "../resources/shaders/screen.rrs");
    m_bloom_downsample_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/bloom_downsample.rrs");
    m_bloom_upsample_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/bloom_upsample.rrs");

    m_final_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/final.rrs");
}

void bloom_app::setup_model()
{
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/mushroom/mushroom.fbx");
    m_material = retro::renderer::material_loader::load_material_from_file("../resources/materials/mushroom.rrm");
}

void bloom_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void bloom_app::setup_screen_quad()
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

    m_screen_vao = std::make_shared<retro::renderer::vertex_array_object>();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);

    std::shared_ptr<retro::renderer::vertex_buffer_object> index_buffer = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements, indices.size());

    m_screen_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
                           quad_vertices.data());

    index_buffer->bind();
    index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size, indices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
        };

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
        vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(
            layout_elements);
    vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

    m_screen_vao->add_vertex_buffer(vertices_vbo);
    m_screen_vao->set_index_buffer(index_buffer);
    m_screen_vao->un_bind();
}

void bloom_app::setup_frame_buffers()
{
    // 1. Create geometry fbo.
    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Albedo
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Normals
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Roughness Metallic AO
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Emissive
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size}};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_internal_format::depth_component32f,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge, viewport_size};
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
        m_geometry_fbo->initialize();
    }
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_internal_format::rgba16f,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
        };
        m_lighting_fbo = std::make_shared<retro::renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y);
        m_lighting_fbo->initialize();
    }
}

void bloom_app::setup_bloom()
{
    glm::vec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    glm::ivec2 viewport_size_int = retro::renderer::renderer::get_viewport_size();

    m_bloom_mips.clear();
    m_bloom_mips.resize(m_bloom_sample_count);

    // Create the bloom fbo
    m_bloom_fbo = std::make_shared<retro::renderer::frame_buffer>(retro::renderer::renderer::get_viewport_size().x, retro::renderer::renderer::get_viewport_size().y);

    // Setup bloom fbo
    for (GLuint i = 0; i < m_bloom_sample_count; i++)
    {
        bloom_mip_data bloom_mip{};
        viewport_size *= 0.5f;
        viewport_size_int /= 2;

        bloom_mip.size = viewport_size_int;

        bloom_mip.texture =
            std::make_shared<retro::renderer::texture>(std::format("bloom_mip_{}", i),
                                                       retro::renderer::texture_data(bloom_mip.size.x, bloom_mip.size.y, 3, 1,
                                                                                     retro::renderer::texture_internal_format::r11g11b10,
                                                                                     retro::renderer::texture_type::none, nullptr));

        m_bloom_mips[i] = bloom_mip;
    }

    // Attach first bloom mip texture to rbo.
    m_bloom_fbo->attach_texture(m_bloom_mips[0].texture, GL_FRAMEBUFFER, retro::renderer::render_buffer_attachment_type::color, GL_TEXTURE_2D, 0);
    m_bloom_fbo->initialize();
}


void bloom_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(bloom_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(bloom_app::on_window_resize));
}

bool bloom_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size());
    m_lighting_fbo->resize(resize_event.get_size());
    setup_bloom();
    m_bloom_fbo->resize(resize_event.get_size(), false);
    return application::on_window_resize(resize_event);
}

bool bloom_app::on_key_pressed(retro::events::key_pressed_event &key_pressed_event)
{
    if (key_pressed_event.get_key_code() == retro::key::D1)
    {
        m_final_render_target = m_lighting_fbo->get_attachment_id(0);
        return true;
    }
    if (key_pressed_event.get_key_code() == retro::key::D2)
    {
        m_final_render_target = m_geometry_fbo->get_attachment_id(0);
        return true;
    }
    if (key_pressed_event.get_key_code() == retro::key::D3)
    {
        m_final_render_target = m_geometry_fbo->get_attachment_id(1);
        return true;
    }
    if (key_pressed_event.get_key_code() == retro::key::D4)
    {
        m_final_render_target = m_geometry_fbo->get_attachment_id(2);
        return true;
    }
    if (key_pressed_event.get_key_code() == retro::key::D5)
    {
        m_final_render_target = m_geometry_fbo->get_attachment_id(3);
        return true;
    }
    if (key_pressed_event.get_key_code() == retro::key::D6)
    {
        m_final_render_target = m_geometry_fbo->get_attachment_id(4);
        return true;
    }
    return false;
}

retro::core::application *retro::core::create_application()
{
    return new bloom_app();
}
