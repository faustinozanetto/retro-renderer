#pragma once

#include "retro.h"

class ssao_app : public retro::core::application
{
public:
    ssao_app();
    ~ssao_app() override;

    void on_update() override;

    void load_shaders();
    void load_texture();
    void setup_model();
    void setup_camera();

    void setup_ssao();

private:
    std::shared_ptr<retro::camera::camera> m_camera;
    std::shared_ptr<retro::renderer::texture> m_texture;
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::model> m_model;
    std

        std::vector<glm::vec3>
            m_ssao_kernel;
    uint32_t m_ssao_noise_texture;
    uint32_t m_ssao_fbo;
    uint32_t m_ssao_color_buffer;
};
