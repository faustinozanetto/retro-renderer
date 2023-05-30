#pragma once

#include "retro.h"

class simple_fonts_app : public retro::core::application
{
public:
    simple_fonts_app();
    ~simple_fonts_app() override;

    void on_update() override;

    void load_shaders();
    void load_font();
    void setup_triangle();

private:
    /* Common Variables */
    std::shared_ptr<retro::renderer::shader> m_shader;
    std::shared_ptr<retro::renderer::vertex_array_object> m_triangle_vao;
    std::shared_ptr<retro::renderer::font> m_font;
    std::shared_ptr<retro::renderer::text> m_test_text;
    std::vector<std::shared_ptr<retro::renderer::text>> m_texts;
};
