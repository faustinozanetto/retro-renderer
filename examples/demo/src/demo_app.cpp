#include "demo_app.h"

#include "core/entry_point.h"

demo_app::demo_app()
{
    retro::renderer::window *window = new retro::renderer::window(1920, 1080, "Retro Renderer");
}

demo_app::~demo_app()
{
}

retro::core::application *retro::core::create_application()
{
    return new demo_app();
}
