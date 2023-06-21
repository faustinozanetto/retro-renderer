#include "editor_camera_panel.h"

#include "../editor_app.h"
#include "../utils/editor_ui_utils.h"

#include <core/application.h>

namespace retro::editor
{
    editor_camera_panel::editor_camera_panel(): editor_panel("Camera")
    {
    }

    editor_camera_panel::~editor_camera_panel() {}

    void editor_camera_panel::on_render_panel()
    {
		RT_PROFILE;
        auto app = dynamic_cast<editor_app *>(&core::application::get());
        auto camera = app->get_main_layer()->get_camera();

		ImGui::Begin("Camera", &m_show);

		glm::vec3 location = camera->get_position();
		if (editor_ui_utils::draw_property("Position", location, -25.0f, 25.0f, 0.05f))
		{
			camera->set_position(location);
		}

		float camera_fov = camera->get_fov();
		if (editor_ui_utils::draw_property("Field of View", camera_fov, 1.0f, 120.0f, 0.1f))
		{
			camera->set_fov(camera_fov);
		}

		float camera_near_plane = camera->get_near_plane();
		if (editor_ui_utils::draw_property("Near Plane", camera_near_plane, 0.01f, 100.0f, 0.1f))
		{
			camera->set_near_plane(camera_near_plane);
		}

		float camera_far_plane = camera->get_far_plane();
		if (editor_ui_utils::draw_property("Far Plane", camera_far_plane, 1.0f, 5000.0f, 1.0f))
		{
			camera->set_far_plane(camera_far_plane);
		}
		ImGui::End();
    }
}