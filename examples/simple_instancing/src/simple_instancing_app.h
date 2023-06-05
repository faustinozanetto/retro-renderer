#pragma once

#include "retro.h"

class simple_instancing_app : public retro::core::application
{
public:
    simple_instancing_app();
    ~simple_instancing_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_cube();
    void setup_instancing();
    void setup_camera();

    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_cube_vao;

    /* Instancing Variables */
    int m_instances_count;
    std::shared_ptr<retro::renderer::shader> m_instancing_shader;
};
