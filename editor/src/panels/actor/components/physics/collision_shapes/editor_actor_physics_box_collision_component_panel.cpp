#include "editor_actor_physics_box_collision_component_panel.h"

#include <memory>

namespace retro::editor
{
	editor_actor_physics_box_collision_component_panel::editor_actor_physics_box_collision_component_panel() : editor_actor_component_panel("Physics Box Collision Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_box_collision_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component =  editor_main_layer::s_selected_actor.has_component<scene::physics_box_collision_shape_component>();
		auto component_hash = typeid(scene::physics_box_collision_shape_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_box_collision_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto & physics_box_collision_shape_component =  editor_main_layer::s_selected_actor.get_component<scene::physics_box_collision_shape_component>();

		glm::vec3 half_extents = physics_box_collision_shape_component.get_box_collision()->get_half_extents();
		if (editor_ui_utils::draw_property("Half Extents", half_extents, 0.0f, 20.0f, 0.01f)) {

			physics_box_collision_shape_component.get_box_collision()->set_half_extents(half_extents);
		}
	}

	void editor_actor_physics_box_collision_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::physics_box_collision_shape_component>();
	}
}
