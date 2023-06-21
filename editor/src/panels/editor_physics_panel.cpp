#include "editor_physics_panel.h"

#include "../editor_app.h"
#include "../utils/editor_ui_utils.h"

#include <core/application.h>

namespace retro::editor
{
	editor_physics_panel::editor_physics_panel(): editor_panel("Physics")
	{
	}

	editor_physics_panel::~editor_physics_panel() {}

	void editor_physics_panel::on_render_panel()
	{
		RT_PROFILE;

		ImGui::Begin("Physics", &m_show);
		ImGui::End();
		/*
		if (!physics::physics_world::get_initialized()) return;

		auto physics_world = physics::physics_world::get();

		ImGui::Begin("Physics");
		glm::vec3 gravity = physics_world.get_gravity();
		if (editor_ui_utils::draw_property("Gravity", gravity, -15.0f, 15.0f, 0.001f))
		{
			physics_world.set_gravity(gravity);
		}
		ImGui::End();
	*/
	}
}