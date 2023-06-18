#include "editor_actor_physics_plane_collision_component_panel.h"

namespace retro::editor
{
	editor_actor_physics_plane_collision_component_panel::editor_actor_physics_plane_collision_component_panel() : editor_actor_component_panel("Physics Plane Collision Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_plane_collision_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component =  editor_main_layer::s_selected_actor.has_component<scene::physics_plane_collision_shape_component>();
		auto component_hash = typeid(scene::physics_plane_collision_shape_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_plane_collision_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto &physics_plane_collision_shape_component =  editor_main_layer::s_selected_actor.get_component<scene::physics_plane_collision_shape_component>();
	}
}