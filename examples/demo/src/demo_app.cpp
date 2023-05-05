#include "demo_app.h"

#include "core/entry_point.h"

demo_app::demo_app()
{
}

demo_app::~demo_app()
{
}

retro::core::application *retro::core::create_application()
{
    return new demo_app();
}
