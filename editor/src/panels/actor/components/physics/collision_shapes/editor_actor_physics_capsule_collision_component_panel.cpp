#include "editor_actor_physics_capsule_collision_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <utils/files.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_physics_capsule_collision_component_panel::editor_actor_physics_capsule_collision_component_panel() : editor_actor_component_panel("Physics Capsule Collision Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_capsule_collision_component_panel::get_actor_component_details()
	{
		const auto& current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::physics_capsule_collision_shape_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::physics_capsule_collision_shape_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_capsule_collision_component_panel::on_render_component_details()
	{
		const auto& current_scene = scene::scene_manager::get().get_active_scene();

		auto &physics_capsule_collision_shape_component = current_scene->get_actors_registry()->get<scene::physics_capsule_collision_shape_component>(
			editor_main_layer::s_selected_actor);

		float half_height = physics_capsule_collision_shape_component.get_capsule_collision()->get_half_height();
		if (editor_ui_utils::draw_property("Half Height", half_height, 0.0f, 20.0f, 0.01f))
		{
			physics_capsule_collision_shape_component.get_capsule_collision()->set_half_height(half_height);
		}

		float radius = physics_capsule_collision_shape_component.get_capsule_collision()->get_radius();
		if (editor_ui_utils::draw_property("Radius", radius, 0.0f, 20.0f, 0.01f))
		{
			physics_capsule_collision_shape_component.get_capsule_collision()->set_radius(radius);
		}
	}
}