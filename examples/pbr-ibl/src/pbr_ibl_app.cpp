#include "rtpch.h"
#include "pbr_ibl_app.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <imgui.h>
#include <core/entry_point.h>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

pbr_ibl_app::pbr_ibl_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
    setup_debug_model();
    setup_camera();
    setup_fbo();
    setup_screen_quad();
    setup_light();
    setup_environment();
}

pbr_ibl_app::~pbr_ibl_app()
{
}

void pbr_ibl_app::on_update()
{
    // 1. Render to geometry buffer
    m_geometry_fbo->bind();
    retro::renderer::renderer::clear_screen();
    m_geometry_shader->bind();
    m_geometry_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_geometry_shader->set_mat4("u_projection", m_camera->get_projection_matrix());

    {
        glm::mat4 model = glm::mat4(1.0f);
        //  model = glm::rotate(model, static_cast<float>(glfwGetTime()), {0, 1, 0});
        model = glm::translate(model, m_object_pos);
        model = glm::scale(model, glm::vec3(3));
        const glm::mat4 rotation = glm::toMat4(glm::quat(m_object_rot));
        model *= rotation;
        m_geometry_shader->set_mat4("u_transform", model);
        m_material->bind(m_geometry_shader);
        retro::renderer::renderer::submit_model(m_model);
    }
    {
        glm::mat4 model = glm::mat4(1.0f);
        m_geometry_shader->set_mat4("u_transform", model);
        m_debug_material->bind(m_geometry_shader);
        retro::renderer::renderer::submit_model(m_debug_sphere);
    }

    /*
     {
         int nrRows = 7;
         int nrColumns = 7;
         float spacing = 2.5f;
         glm::mat4 model = glm::mat4(1.0f);
         for (int row = 0; row < nrRows; ++row)
         {
             m_debug_material->set_metallic((float)row / (float)nrRows);
             for (int col = 0; col < nrColumns; ++col)
             {
                 // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                 // on direct lighting.
                 m_debug_material->set_roughness(glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
                 model = glm::mat4(1.0f);
                 model = glm::translate(model, glm::vec3(
                                                   (float)(col - (nrColumns / 2)) * spacing,
                                                   (float)(row - (nrRows / 2)) * spacing,
                                                   -2.0f));
                 m_debug_material->set_albedo({0.5f, 0.0f, 0.0f});
                 m_geometry_shader->set_mat4("u_transform", model);
                 m_debug_material->bind(m_geometry_shader);
                 retro::renderer::renderer::submit_model(m_debug_sphere);
             }
         }
     }
     */
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_point_light->get_position());
        model = glm::scale(model, glm::vec3(0.2f));
        m_geometry_shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_model(m_light_model);
    }
    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();

    // 2. Render unsing the pbr shader
    m_final_fbo->bind();
    m_lighting_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0));              // Position
    retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1));              // Albedo
    retro::renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2));              // Normal
    retro::renderer::renderer::bind_texture(3, m_geometry_fbo->get_attachment_id(3));              // Roughmetalao
    retro::renderer::renderer::bind_texture(4, m_environment_irradiance_texture->get_handle_id()); // Irradiance
    retro::renderer::renderer::bind_texture(5, m_environment_prefilter_texture->get_handle_id());  // Prefilter
    retro::renderer::renderer::bind_texture(6, m_environment_brdf_texture);                        // BRDF

    m_lighting_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_lighting_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_lighting_shader->set_vec_float3("p_light.position", m_point_light->get_position());
    m_lighting_shader->set_vec_float3("p_light.diffuse", m_point_light->get_diffuse());
    m_lighting_shader->set_vec_float3("p_light.specular", m_point_light->get_specular());
    m_lighting_shader->set_int("u_use_irradiance", m_use_irradiance ? 1 : 0);
    m_lighting_shader->set_vec_float3("u_cam_pos", m_camera->get_position());
    m_lighting_shader->set_mat4("u_view", m_camera->get_view_matrix());
    m_lighting_shader->set_mat4("u_projection", m_camera->get_projection_matrix());

    m_quad_vao->bind();
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_quad_vao->un_bind();

    m_lighting_shader->un_bind();
    m_final_fbo->bind();

    // 3. Render skybox
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_geometry_fbo->get_handle_id());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_final_fbo->get_handle_id());
    auto w = m_final_fbo->get_width();
    auto h = m_final_fbo->get_height();
    glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    render_skybox();
    glDepthFunc(GL_LESS);

    m_final_fbo->un_bind();

    // 4. Render final screen quad
    m_screen_shader->bind();
    m_quad_vao->bind();
    retro::renderer::renderer::bind_texture(0, m_final_fbo->get_attachment_id(0));
    retro::renderer::renderer::submit_elements(GL_TRIANGLES, 6);
    m_quad_vao->un_bind();
    m_screen_shader->un_bind();

    retro::ui::engine_ui::begin_frame();

    ImGui::Begin("Environment");
    bool use_irradiance = m_use_irradiance;
    if (ImGui::Checkbox("Irradiance Map", &use_irradiance))
    {
        m_use_irradiance = use_irradiance;
    }
    ImGui::End();

    ImGui::Begin("Debug Model");

    float roughness = m_debug_roughness;
    if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f))
    {
        m_debug_roughness = roughness;
    }
    float metallic = m_debug_metallic;
    if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f))
    {
        m_debug_metallic = metallic;
    }
    m_debug_material->set_roughness(m_debug_roughness);
    m_debug_material->set_metallic(m_debug_metallic);
    ImGui::End();

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

    ImGui::Begin("Camera");
    glm::vec3 cam_pos = m_camera->get_position();
    if (ImGui::SliderFloat3("Camera Pos", glm::value_ptr(cam_pos), -15.0f, 15.0f))
    {
        m_camera->set_position(cam_pos);
    }
    ImGui::End();
    ImGui::Begin("Light");
    glm::vec3 position = m_point_light->get_position();
    if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f))
    {
        m_point_light->set_position(position);
    }
    glm::vec3 diffuse = m_point_light->get_diffuse();
    if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse)))
    {
        m_point_light->set_diffuse(diffuse);
    }
    glm::vec3 specular = m_point_light->get_specular();
    if (ImGui::ColorEdit3("Specular", glm::value_ptr(specular)))
    {
        m_point_light->set_specular(specular);
    }
    ImGui::End();
    retro::ui::engine_ui::end_frame();
}

void pbr_ibl_app::load_shaders()
{
    m_geometry_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/geometry.rrs");

    m_lighting_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/lighting.rrs");

    m_equirectangular_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/equirectangular.rrs");

    m_irradiance_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/irradiance.rrs");

    m_skybox_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/skybox.rrs");

    m_screen_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/screen.rrs");

    m_brdf_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/brdf.rrs");

    m_prefilter_shader = retro::renderer::shader_loader::load_shader_from_file(
        "resources/shaders/prefilter.rrs");
}

void pbr_ibl_app::load_texture()
{
    m_albedo_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/models/crate/textures/crate_baseColor.jpeg");
    m_normal_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/models/crate/textures/crate_normal.jpeg");
    m_roughness_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/models/crate/textures/crate_metallicRoughness.png");
    m_metallic_texture = retro::renderer::texture_loader::load_texture_from_file(
        "resources/models/crate/textures/crate_metallicRoughness.png");
    //  m_ao_texture = retro::renderer::texture_loader::load_texture_from_file("resources/models/crate/textures/tv-ao.jpeg");
}

void pbr_ibl_app::setup_model()
{
    m_object_pos = glm::vec3(0);
    m_object_rot = glm::vec3(0);
    m_model = retro::renderer::model_loader::load_model_from_file("resources/models/crate/scene.gltf");

    retro::renderer::material_texture albedo;
    albedo.texture = m_albedo_texture;
    albedo.is_enabled = true;
    albedo.type = retro::renderer::material_texture_type::albedo;

    retro::renderer::material_texture normal;
    normal.texture = m_normal_texture;
    normal.is_enabled = true;
    normal.type = retro::renderer::material_texture_type::normal;

    retro::renderer::material_texture roughness;
    roughness.texture = m_roughness_texture;
    roughness.is_enabled = true;
    roughness.type = retro::renderer::material_texture_type::roughness;

    retro::renderer::material_texture metallic;
    metallic.texture = m_metallic_texture;
    metallic.is_enabled = true;
    metallic.type = retro::renderer::material_texture_type::metallic;

    retro::renderer::material_texture ambient_occlusion;
    ambient_occlusion.texture = nullptr;
    ambient_occlusion.is_enabled = false;
    ambient_occlusion.type = retro::renderer::material_texture_type::ambient_occlusion;

    std::map<retro::renderer::material_texture_type, int> material_bindings;
    material_bindings[retro::renderer::material_texture_type::albedo] = 0;
    material_bindings[retro::renderer::material_texture_type::normal] = 1;
    material_bindings[retro::renderer::material_texture_type::roughness] = 2;
    material_bindings[retro::renderer::material_texture_type::metallic] = 3;
    material_bindings[retro::renderer::material_texture_type::ambient_occlusion] = 4;

    std::unordered_map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures;
    textures[retro::renderer::material_texture_type::albedo] = albedo;
    textures[retro::renderer::material_texture_type::normal] = normal;
    textures[retro::renderer::material_texture_type::roughness] = roughness;
    textures[retro::renderer::material_texture_type::metallic] = metallic;
    textures[retro::renderer::material_texture_type::ambient_occlusion] = ambient_occlusion;

    m_material = std::make_shared<retro::renderer::material>(textures, material_bindings);
    m_material->set_ambient_occlusion(1.0f);
}

void pbr_ibl_app::setup_debug_model()
{
    m_debug_roughness = 1.0f;
    m_debug_metallic = 1.0f;
    m_debug_sphere = retro::renderer::model_loader::load_model_from_file("../resources/models/sphere.obj");

    std::map<retro::renderer::material_texture_type, int> material_bindings;
    material_bindings[retro::renderer::material_texture_type::albedo] = 0;
    material_bindings[retro::renderer::material_texture_type::normal] = 1;
    material_bindings[retro::renderer::material_texture_type::roughness] = 2;
    material_bindings[retro::renderer::material_texture_type::metallic] = 3;
    material_bindings[retro::renderer::material_texture_type::ambient_occlusion] = 4;

    m_debug_material = std::make_shared<retro::renderer::material>(material_bindings);
    m_debug_material->set_albedo(glm::vec3(1));
}

void pbr_ibl_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f,
                                                       1000.0f);
    m_camera->set_position({0.0f, 0.5f, 12.0f});
}

void pbr_ibl_app::setup_fbo()
{
    // 1. Create geometry fbo.
    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Albedo
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Normals
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
            // Roughness Metallic AO
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size}};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_format::depth_component32f,
            retro::renderer::texture_internal_format::rgba,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge, viewport_size};
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(
            attachments, viewport_size.x, viewport_size.y, depth_attachment);
    }

    // 2. Create final fbo
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge, viewport_size},
        };
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_format::depth_component32f,
            retro::renderer::texture_internal_format::rgba,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge, viewport_size};
        m_final_fbo = std::make_shared<retro::renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y,
                                                                      depth_attachment);
    }
}

void pbr_ibl_app::setup_screen_quad()
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
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements,
                                               indices.size());

    m_quad_vao->bind();
    vertices_vbo->bind();
    vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, vertex_buffer_size,
                           quad_vertices.data());

    index_buffer->bind();
    index_buffer->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, index_buffer_size,
                           indices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false},
        };

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor>
        vertices_vbo_layout_descriptor = std::make_shared<retro::renderer::vertex_buffer_layout_descriptor>(
            layout_elements);
    vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

    m_quad_vao->add_vertex_buffer(vertices_vbo);
    m_quad_vao->set_index_buffer(index_buffer);
    m_quad_vao->un_bind();
}

void pbr_ibl_app::setup_light()
{
    m_point_light = std::make_shared<retro::renderer::point_light>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.85f),
                                                                   glm::vec3(1.0f));
    m_light_model = retro::renderer::model_loader::load_model_from_file("../resources/models/cube.obj");
}

void pbr_ibl_app::setup_enviroment_cube()
{
    std::vector<float>
        cube_vertices = {
            // back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
            // front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
            // right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
        };
    size_t size = cube_vertices.size() * sizeof(&cube_vertices[0]);

    m_environment_cube_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_environment_cube_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> cube_vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    cube_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, cube_vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_normal", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> cube_vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    cube_vbo->set_layout_descriptor(cube_vbo_layout_descriptor);

    m_environment_cube_vao->add_vertex_buffer(cube_vbo);
}

void pbr_ibl_app::setup_environment_quad()
{
    std::vector<float>
        vertices = {
            // positions        // texture Coords
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
        };
    size_t size = vertices.size() * sizeof(&vertices[0]);

    m_environment_quad_vao = std::make_shared<retro::renderer::vertex_array_object>();
    m_environment_quad_vao->bind();
    std::shared_ptr<retro::renderer::vertex_buffer_object> vbo = std::make_shared<
        retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
    vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());

    std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
            {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

    std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> vbo_layout_descriptor = std::make_shared<
        retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_environment_quad_vao->add_vertex_buffer(vbo);
}

void pbr_ibl_app::setup_environment_cubemap()
{
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    retro::renderer::texture_data texture_data = {
        m_environment_map_size, m_environment_map_size, 3, retro::renderer::texture_type::cubemap, nullptr};
    m_environment_cubemap_texture = std::make_shared<retro::renderer::texture>("environment", texture_data);
}

void pbr_ibl_app::setup_environment_fbo()
{
    // pbr: setup framebuffer
    // ----------------------
    glm::ivec2 viewport_size = retro::renderer::renderer::get_viewport_size();
    m_environment_capture_fbo = std::make_shared<retro::renderer::frame_buffer>(viewport_size.x, viewport_size.y);
    m_environment_capture_fbo->bind(false);
    m_environment_capture_rbo = std::make_shared<retro::renderer::render_buffer>(
        m_environment_map_size, m_environment_map_size, retro::renderer::texture_format::depth_component24);
    m_environment_capture_rbo->attach_to_frame_buffer(retro::renderer::render_buffer_attachment_type::depth);
}

void pbr_ibl_app::setup_environment()
{
    setup_enviroment_cube();
    setup_environment_quad();

    // pbr: load the HDR environment map
    // ---------------------------------
    m_environment_hdr_texture = retro::renderer::texture_loader::load_texture_cubemap_from_file(
        "resources/textures/je_gray_park_4k.hdr");
    m_irradiance_map_size = 128;
    m_prefilter_map_size = 1024;
    m_brdf_map_size = 1024;
    m_environment_map_size = 2048;
    m_use_irradiance = true;

    setup_environment_fbo();
    setup_environment_cubemap();
    setup_environment_equirectangular_map();
    setup_environment_irradiance_map();
    setup_environment_prefilter_map();
    setup_environment_brdf_map();

    retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
}

void pbr_ibl_app::setup_environment_equirectangular_map()
{
    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    m_capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    m_capture_views = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    m_equirectangular_shader->bind();
    m_equirectangular_shader->set_mat4("u_projection", m_capture_projection);
    retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

    retro::renderer::renderer::set_viewport_size({m_environment_map_size, m_environment_map_size});
    m_environment_capture_fbo->bind(false);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_equirectangular_shader->set_mat4("u_view", m_capture_views[i]);
        m_environment_capture_fbo->attach_texture(m_environment_cubemap_texture, GL_FRAMEBUFFER,
                                                  retro::renderer::render_buffer_attachment_type::color,
                                                  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0);
        retro::renderer::renderer::clear_screen();

        retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    }
    m_environment_capture_fbo->un_bind();

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_environment_cubemap_texture->get_handle_id());
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void pbr_ibl_app::setup_environment_irradiance_map()
{
    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    retro::renderer::texture_data texture_data = {
        m_irradiance_map_size, m_irradiance_map_size, 3, retro::renderer::texture_type::cubemap, nullptr};
    m_environment_irradiance_texture = std::make_shared<retro::renderer::texture>("irradiance", texture_data);

    m_environment_capture_fbo->bind(false);
    m_environment_capture_rbo->bind();
    m_environment_capture_rbo->set_storage_parameters(m_irradiance_map_size, m_irradiance_map_size,
                                                      retro::renderer::texture_format::depth_component24);

    m_irradiance_shader->bind();
    m_irradiance_shader->set_mat4("u_projection", m_capture_projection);
    retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

    retro::renderer::renderer::set_viewport_size({m_irradiance_map_size, m_irradiance_map_size});
    m_environment_capture_fbo->bind(false);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_irradiance_shader->set_mat4("u_view", m_capture_views[i]);
        m_environment_capture_fbo->attach_texture(m_environment_irradiance_texture, GL_FRAMEBUFFER,
                                                  retro::renderer::render_buffer_attachment_type::color,
                                                  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0);
        retro::renderer::renderer::clear_screen();

        retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    }
    m_environment_capture_fbo->un_bind();
}

void pbr_ibl_app::setup_environment_prefilter_map()
{
    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    // --------------------------------------------------------------------------------
    retro::renderer::texture_data texture_data = {
        m_prefilter_map_size, m_prefilter_map_size, 3, retro::renderer::texture_type::cubemap, nullptr};
    m_environment_prefilter_texture = std::make_shared<retro::renderer::texture>("pre_filter", texture_data);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    m_prefilter_shader->bind();
    m_prefilter_shader->set_mat4("u_projection", m_capture_projection);
    retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

    m_environment_capture_fbo->bind(false);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = static_cast<unsigned int>(m_prefilter_map_size * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(m_prefilter_map_size * std::pow(0.5, mip));
        m_environment_capture_rbo->bind();
        m_environment_capture_rbo->set_storage_parameters(mipWidth, mipHeight,
                                                          retro::renderer::texture_format::depth_component24);
        retro::renderer::renderer::set_viewport_size({mipWidth, mipHeight});

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_prefilter_shader->set_float("u_roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_prefilter_shader->set_mat4("u_view", m_capture_views[i]);
            m_environment_capture_fbo->attach_texture(m_environment_prefilter_texture, GL_FRAMEBUFFER,
                                                      retro::renderer::render_buffer_attachment_type::color,
                                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip);
            retro::renderer::renderer::clear_screen();

            retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
        }
    }
    m_environment_capture_fbo->un_bind();
}

void pbr_ibl_app::setup_environment_brdf_map()
{
    // pbr: generate a 2D LUT from the BRDF equations used.
    // ----------------------------------------------------
    /*
    retro::renderer::texture_data texture_data = {m_brdf_map_size, m_brdf_map_size, 2, {retro::renderer::texture_format::rg16f, retro::renderer::texture_internal_format::rg}, retro::renderer::texture_type::normal, nullptr};
    m_environment_brdf_texture = std::make_shared<retro::renderer::texture>(texture_data);
    */
    glGenTextures(1, &m_environment_brdf_texture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, m_environment_brdf_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_brdf_map_size, m_brdf_map_size, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_environment_capture_fbo->bind();
    m_environment_capture_rbo->bind();
    m_environment_capture_rbo->set_storage_parameters(m_brdf_map_size, m_brdf_map_size,
                                                      retro::renderer::texture_format::depth_component24);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_environment_brdf_texture, 0);
    //   m_environment_capture_fbo->attach_texture(m_environment_brdf_texture, GL_FRAMEBUFFER, retro::renderer::render_buffer_attachment_type::color, GL_TEXTURE_2D, 0);

    retro::renderer::renderer::set_viewport_size({m_brdf_map_size, m_brdf_map_size});
    m_brdf_shader->bind();
    retro::renderer::renderer::clear_screen();
    m_environment_quad_vao->bind();
    retro::renderer::renderer::submit_arrays(GL_TRIANGLE_STRIP, 4);
    m_environment_quad_vao->un_bind();
    m_environment_capture_fbo->un_bind();
}

void pbr_ibl_app::render_skybox()
{
    m_skybox_shader->bind();
    // retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());
    //  retro::renderer::renderer::bind_texture(0, m_environment_irradiance_texture->get_handle_id());
    retro::renderer::renderer::bind_texture(0, m_environment_prefilter_texture->get_handle_id());
    m_skybox_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_skybox_shader->set_mat4("u_view", m_camera->get_view_matrix());
    retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    m_skybox_shader->un_bind();
}

void pbr_ibl_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    //  dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(pbr_ibl_app::on_resize_ssao));
}

bool pbr_ibl_app::on_window_resize(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size());
    m_final_fbo->resize(resize_event.get_size());
    return application::on_window_resize(resize_event);
}

retro::core::application *retro::core::create_application()
{
    return new pbr_ibl_app();
}
