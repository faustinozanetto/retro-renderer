#include "editor_actor_name_component_panel.h"

#include "../../../editor_main_layer.h"

#include <core/application.h>
#include <scene/scene_manager.h>

namespace retro::editor
{
	editor_actor_name_component_panel::editor_actor_name_component_panel()
	{
	}

	editor_actor_name_component_panel::~editor_actor_name_component_panel() {}

	void editor_actor_name_component_panel::on_render_panel()
	{
		auto current_scene = scene::scene_manager::get().get_active_scene();

		if (!current_scene->get_actors_registry()->any_of<scene::name_component>(editor_main_layer::s_selected_actor))
			return;

		auto &name_component = current_scene->get_actors_registry()->get<scene::name_component>(
			editor_main_layer::s_selected_actor);

		// Editable name field.
		char buffer[256] = {};
		strcpy_s(buffer, name_component.get_name().c_str());
		if (ImGui::InputText("##name", buffer, sizeof(buffer)))
		{
			name_component.set_name(std::string(buffer));
		}

		// Actor handle
		ImGui::SameLine();
		ImGui::Text("Actor Handle: %s",
					std::to_string(static_cast<uint32_t>(editor_main_layer::s_selected_actor)).c_str());
	}
}