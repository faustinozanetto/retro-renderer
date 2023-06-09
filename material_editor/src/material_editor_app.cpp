#include "material_editor_app.h"

#include <core/entry_point.h>
#include <core/application.h>

namespace retro::material_editor
{

    material_editor_app::material_editor_app() : application("./")
    {
    }

    material_editor_app::~material_editor_app()
    {
    }

    void material_editor_app::on_update()
    {
    }

    void material_editor_app::on_handle_event(retro::events::base_event &event)
    {
    }

    bool material_editor_app::on_window_resize(retro::events::window_resize_event &resize_event)
    {
        return application::on_window_resize(resize_event);
    }
}

retro::core::application* retro::core::create_application()
{
	return new retro::material_editor::material_editor_app();
}