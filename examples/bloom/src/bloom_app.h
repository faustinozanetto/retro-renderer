#pragma once

#include "retro.h"

#include <imgui.h>

#include <imgui_node_editor.h>

namespace ImGuiNodeEditor = ax::NodeEditor;

retro::renderer::material_texture_type material_texture_types_array[] = {
    retro::renderer::material_texture_type::albedo,
    retro::renderer::material_texture_type::normal,
    retro::renderer::material_texture_type::roughness,
    retro::renderer::material_texture_type::metallic,
    retro::renderer::material_texture_type::ambient_occlusion,
    retro::renderer::material_texture_type::emissive,
    retro::renderer::material_texture_type::opacity};

struct bloom_mip_data
{
    glm::ivec2 size;
    std::shared_ptr<retro::renderer::texture> texture;
};

struct editor_link_info
{
    ImGuiNodeEditor::LinkId id;
    ImGuiNodeEditor::PinId  input_id;
    ImGuiNodeEditor::PinId  output_id;
};


class bloom_app : public retro::core::application
{
public:
    bloom_app();
    ~bloom_app() override;

    void on_update() override;

    void load_shaders();
    void setup_model();
    void setup_camera();
    void setup_screen_quad();
    void setup_frame_buffers();
    void setup_bloom();

    void draw_editor();

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;
    bool on_key_pressed(retro::events::key_pressed_event &key_pressed_event);

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    ImGuiNodeEditor::EditorContext *m_editor_context = nullptr;
    ImVector<editor_link_info>   m_editor_links;
    int                  m_editor_next_link_id = 100;

    /* Bloom */
    int m_bloom_sample_count;
    std::shared_ptr<retro::renderer::frame_buffer> m_bloom_fbo;
    std::shared_ptr<retro::renderer::shader> m_bloom_downsample_shader;
    std::shared_ptr<retro::renderer::shader> m_bloom_upsample_shader;
    std::vector<bloom_mip_data> m_bloom_mips;
    float m_filter_radius;

    /* Rendering */
    int m_final_render_target;
    std::shared_ptr<retro::renderer::frame_buffer> m_geometry_fbo;
    std::shared_ptr<retro::renderer::frame_buffer> m_lighting_fbo;
    std::shared_ptr<retro::renderer::shader> m_geometry_shader;
    std::shared_ptr<retro::renderer::shader> m_screen_shader;
    std::shared_ptr<retro::renderer::shader> m_final_shader;
    std::shared_ptr<retro::renderer::shader> m_pbr_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_screen_vao;

    /* Model Variables */
    glm::vec3 m_model_pos;
    std::shared_ptr<retro::renderer::material> m_material;
    std::shared_ptr<retro::renderer::model> m_model;
};
