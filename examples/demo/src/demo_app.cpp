#include "demo_app.h"

#include "core/entry_point.h"

demo_app::demo_app() : application("../")
{
}

demo_app::~demo_app()
{
}

void demo_app::on_update()
{
}

retro::core::application *retro::core::create_application()
{
    return new demo_app();
}
