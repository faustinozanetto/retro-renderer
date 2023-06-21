#include "editor_actor_name_component_panel.h"

namespace retro::editor
{
	editor_actor_name_component_panel::editor_actor_name_component_panel() : editor_actor_component_panel("Name Component")
	{
	}

	std::pair<bool, size_t> editor_actor_name_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::name_component>();
		auto component_hash = typeid(scene::name_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}
	
	void editor_actor_name_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		auto &name_component = editor_main_layer::s_selected_actor.get_component<scene::name_component>();

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
					std::to_string(static_cast<uint32_t>(editor_main_layer::s_selected_actor.get_handle())).c_str());
	}

	void editor_actor_name_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::name_component>();
	}
}
