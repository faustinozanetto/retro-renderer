#include "editor_actor_physics_static_actor_component_panel.h"

namespace retro::editor
{
	editor_actor_physics_static_actor_component_panel::editor_actor_physics_static_actor_component_panel() : editor_actor_component_panel("Physics Static Actor Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_static_actor_component_panel::get_actor_component_details()
	{
		bool has_component =  editor_main_layer::s_selected_actor.has_component<scene::physics_static_actor_component>();
		auto component_hash = typeid(scene::physics_static_actor_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_static_actor_component_panel::on_render_component_details()
	{
		const auto &physics_static_actor_component = editor_main_layer::s_selected_actor.get_component<scene::physics_static_actor_component>();
	}

	void editor_actor_physics_static_actor_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::physics_static_actor_component>();
	}
}
