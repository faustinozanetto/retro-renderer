#pragma once

#include "retro.h"

struct bloom_mip_data
{
    glm::vec2 size;
    glm::ivec2 int_size;
    std::shared_ptr<retro::renderer::texture> texture;
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

    void on_handle_event(retro::events::base_event &event) override;
    bool on_window_resize(retro::events::window_resize_event &resize_event) override;
    bool on_key_pressed(retro::events::key_pressed_event &key_pressed_event);

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Bloom */
    int m_bloom_sample_count;
    std::shared_ptr<retro::renderer::frame_buffer> m_bloom_fbo;
    std::shared_ptr<retro::renderer::shader> m_bloom_downsample_shader;
    std::shared_ptr<retro::renderer::shader> m_bloom_upsample_shader;
    std::vector<bloom_mip_data> m_bloom_mips;

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
    std::shared_ptr<retro::renderer::material> m_material;
    std::shared_ptr<retro::renderer::model> m_model;
};
