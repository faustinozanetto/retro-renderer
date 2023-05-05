#include "simple_triangle_app.h"

simple_triangle_app::simple_triangle_app() : application("./")
{
    load_shaders();
}

simple_triangle_app::~simple_triangle_app()
{
}

void simple_triangle_app::load_shaders()
{
    const std::string &shader_contents = retro::renderer::shader_loader::read_shader_from_file("resources/shaders/screen.rrs");
    const auto &shader_sources = retro::renderer::shader_loader::parse_shader_source(shader_contents);
    const std::shared_ptr<retro::renderer::shader> &shader = std::make_shared<retro::renderer::shader>(shader_sources);
}

retro::core::application *retro::core::create_application()
{
    return new simple_triangle_app();
}
