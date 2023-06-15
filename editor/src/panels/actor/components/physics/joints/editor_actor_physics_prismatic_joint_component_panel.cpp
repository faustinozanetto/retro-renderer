#include "editor_actor_physics_prismatic_joint_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_physics_prismatic_joint_component_panel::editor_actor_physics_prismatic_joint_component_panel() : editor_actor_component_panel("Physics Prismatic Joint Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_prismatic_joint_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::physics_prismatic_joint_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::physics_prismatic_joint_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_prismatic_joint_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &physics_prismatic_joint_component = current_scene->get_actors_registry()->get<scene::physics_prismatic_joint_component>(
			editor_main_layer::s_selected_actor);

		float lower_limit = physics_prismatic_joint_component.get_prismatic_joint()->get_limit().lower;

		float upper_limit = physics_prismatic_joint_component.get_prismatic_joint()->get_limit().upper;

		if (editor_ui_utils::draw_property("Lower Limit", lower_limit, -20.0f, 20.0f, 0.01f))
		{
			physics_prismatic_joint_component.get_prismatic_joint()->set_limit(lower_limit, upper_limit);
		}

		if (editor_ui_utils::draw_property("Upper Limit", upper_limit, -20.0f, 20.0f, 0.01f))
		{
			physics_prismatic_joint_component.get_prismatic_joint()->set_limit(lower_limit, upper_limit);
		}
	}
}