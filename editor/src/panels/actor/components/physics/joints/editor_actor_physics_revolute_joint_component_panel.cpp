#include "editor_actor_physics_revolute_joint_component_panel.h"

#include <memory>

namespace retro::editor
{
	editor_actor_physics_revolute_joint_component_panel::editor_actor_physics_revolute_joint_component_panel() : editor_actor_component_panel("Physics Revolute Joint Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_revolute_joint_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::physics_revolute_joint_component>();
		auto component_hash = typeid(scene::physics_revolute_joint_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_revolute_joint_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto &physics_revolute_joint_component =  editor_main_layer::s_selected_actor.get_component<scene::physics_revolute_joint_component>();

		float lower_limit = physics_revolute_joint_component.get_revolute_joint()->get_limit().lower;
		float upper_limit = physics_revolute_joint_component.get_revolute_joint()->get_limit().upper;

		if (editor_ui_utils::draw_property("Lower Limit", lower_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
		{
			physics_revolute_joint_component.get_revolute_joint()->set_limit(lower_limit, upper_limit);
		}

		if (editor_ui_utils::draw_property("Upper Limit", upper_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
		{
			physics_revolute_joint_component.get_revolute_joint()->set_limit(lower_limit, upper_limit);
		}

		float drive_force_limit = physics_revolute_joint_component.get_revolute_joint()->get_drive_force_limit();
		if (editor_ui_utils::draw_property("Drive Force Limit", drive_force_limit, 0.0f, 100.0f, 0.01f))
		{
			physics_revolute_joint_component.get_revolute_joint()->set_drive_force_limit(drive_force_limit);
		}

		float drive_gear_ratio = physics_revolute_joint_component.get_revolute_joint()->get_drive_gear_ratio();
		if (editor_ui_utils::draw_property("Drive Gear Ratio", drive_gear_ratio, 0.0f, 100.0f, 0.01f))
		{
			physics_revolute_joint_component.get_revolute_joint()->set_drive_gear_ratio(drive_gear_ratio);
		}
	}

	void editor_actor_physics_revolute_joint_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::physics_revolute_joint_component>();
	}
}
