#include "rtpch.h"
#include "pbr_ibl_app.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <imgui.h>
#include <stb_image.h>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

pbr_ibl_app::pbr_ibl_app() : application("./")
{
    load_shaders();
    load_texture();
    setup_model();
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
        const glm::mat4 rotation = glm::toMat4(glm::quat(m_object_rot));
        model *= rotation;
        m_geometry_shader->set_mat4("u_transform", model);
        m_material->bind(m_geometry_shader);
        retro::renderer::renderer::submit_model(m_model);
    }
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        m_geometry_shader->set_mat4("u_transform", model);
        retro::renderer::renderer::submit_model(m_light_model);
    }
    m_geometry_shader->un_bind();
    m_geometry_fbo->un_bind();

    // 2. Render unsing the pbr shader
    m_final_fbo->bind();
    m_lighting_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_geometry_fbo->get_attachment_id(0)); // Position
    retro::renderer::renderer::bind_texture(1, m_geometry_fbo->get_attachment_id(1)); // Albedo
    retro::renderer::renderer::bind_texture(2, m_geometry_fbo->get_attachment_id(2)); // Normal
    retro::renderer::renderer::bind_texture(3, m_geometry_fbo->get_attachment_id(3)); // Roughmetalao
    retro::renderer::renderer::bind_texture(4, irradianceMap);                        // Irradiance

    m_lighting_shader->set_vec_float3("p_light.position", m_light_pos);
    m_lighting_shader->set_vec_float3("p_light.color", m_light_color);
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

void pbr_ibl_app::load_shaders()
{
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
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/equirectangular.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_equirectangular_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }

    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/irradiance.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_irradiance_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }

    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/skybox.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_skybox_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
    {
        const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file(
            "resources/shaders/screen.rrs");
        const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
        m_screen_shader = std::make_shared<retro::renderer::shader>(shader_sources);
    }
}

void pbr_ibl_app::load_texture()
{
    m_albedo_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-albedo.png");
    m_normal_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-normal.png");
    m_roughness_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-roughness.png");
    m_metallic_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-metallic.png");
    m_ao_texture = retro::renderer::texture_loader::load_texture_from_file("../resources/models/tv/tv-ao.jpeg");
}

void pbr_ibl_app::setup_model()
{
    m_object_pos = glm::vec3(0);
    m_object_rot = glm::vec3(0);
    m_model = retro::renderer::model_loader::load_model_from_file("../resources/models/tv/tv.obj");

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
    ambient_occlusion.texture = m_ao_texture;
    ambient_occlusion.is_enabled = true;
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

void pbr_ibl_app::setup_camera()
{
    m_camera = std::make_shared<retro::camera::camera>(retro::camera::camera_type::perspective, 45.0f, 0.01f, 1000.0f);
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
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Albedo
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Normals
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
            // Roughness Metallic AO
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            }};
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_format::depth_component32f,
            retro::renderer::texture_internal_format::rgba,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_geometry_fbo = std::make_shared<retro::renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y, depth_attachment);
    }

    // 2. Create final fbo
    {
        std::vector<retro::renderer::frame_buffer_attachment> attachments = {
            //  Position
            {
                retro::renderer::texture_format::rgba16f,
                retro::renderer::texture_internal_format::rgba,
                retro::renderer::texture_filtering::linear,
                retro::renderer::texture_wrapping::clamp_to_edge,
            },
        };
        retro::renderer::frame_buffer_attachment depth_attachment = {
            retro::renderer::texture_format::depth_component32f,
            retro::renderer::texture_internal_format::rgba,
            retro::renderer::texture_filtering::linear,
            retro::renderer::texture_wrapping::clamp_to_edge,
        };
        m_final_fbo = std::make_shared<retro::renderer::frame_buffer>(attachments, viewport_size.x, viewport_size.y, depth_attachment);
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

void pbr_ibl_app::setup_light()
{
    m_light_pos = glm::vec3(0.0f, 0.0f, 5.0f);
    m_light_color = glm::vec3(1.0f);
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
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
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

void pbr_ibl_app::setup_environment_cubemap()
{
    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void pbr_ibl_app::setup_environment_fbo()
{
    // pbr: setup framebuffer
    // ----------------------
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
}

void pbr_ibl_app::setup_environment()
{
    setup_enviroment_cube();

    // pbr: load the HDR environment map
    // ---------------------------------
    m_environment_cubemap_texture = retro::renderer::texture_loader::load_texture_cubemap_from_file("resources/textures/newport_loft.hdr");

    setup_environment_fbo();
    setup_environment_cubemap();
    setup_environment_equirectangular_map();
    setup_environment_irradiance_map();

    retro::renderer::renderer::set_viewport_size(retro::renderer::renderer::get_viewport_size());
}

void pbr_ibl_app::setup_environment_equirectangular_map()
{
    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    captureViews = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    m_equirectangular_shader->bind();
    m_equirectangular_shader->set_int("u_equirectangular_map", 0);
    m_equirectangular_shader->set_mat4("u_projection", captureProjection);
    retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

    retro::renderer::renderer::set_viewport_size({512, 512});
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_equirectangular_shader->set_mat4("u_view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pbr_ibl_app::setup_environment_irradiance_map()
{
    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    m_irradiance_shader->bind();
    m_irradiance_shader->set_int("u_environment_map", 0);
    m_irradiance_shader->set_mat4("u_projection", captureProjection);
    retro::renderer::renderer::bind_texture(0, envCubemap);

    retro::renderer::renderer::set_viewport_size({32, 32});
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_irradiance_shader->set_mat4("u_view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pbr_ibl_app::render_skybox()
{
    m_skybox_shader->bind();
    retro::renderer::renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());
    m_skybox_shader->set_mat4("u_projection", m_camera->get_projection_matrix());
    m_skybox_shader->set_mat4("u_view", m_camera->get_view_matrix());
    retro::renderer::renderer::submit_vao(m_environment_cube_vao, 36);
    m_skybox_shader->un_bind();
}

void pbr_ibl_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::window_resize_event>([this](auto &&...args) -> decltype(auto)
                                                            { return pbr_ibl_app::on_resize_ssao(std::forward<decltype(args)>(args)...); });
}

bool pbr_ibl_app::on_resize_ssao(retro::events::window_resize_event &resize_event)
{
    m_geometry_fbo->resize(resize_event.get_size());
    m_final_fbo->resize(resize_event.get_size());
    return false;
}

retro::core::application *retro::core::create_application()
{
    return new pbr_ibl_app();
}
