#pragma once

#include "retro.h"

class bloom_app : public retro::core::application
{
public:
    bloom_app();
    ~bloom_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_camera();

    void on_handle_event(retro::events::base_event& event) override;
    bool on_window_resize(retro::events::window_resize_event& resize_event) override;

private:
    /* Common Variables */
    std::shared_ptr<retro::camera::camera> m_camera;

    /* Model Variables */
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::model> m_model;
};
