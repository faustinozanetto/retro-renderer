#include "editor_actor_physics_spherical_joint_component_panel.h"

#include <memory>

namespace retro::editor
{
	editor_actor_physics_spherical_joint_component_panel::editor_actor_physics_spherical_joint_component_panel() : editor_actor_component_panel("Physics Spherical Joint Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_spherical_joint_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::physics_spherical_joint_component>();
		auto component_hash = typeid(scene::physics_spherical_joint_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_spherical_joint_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto &physics_spherical_joint_component = editor_main_layer::s_selected_actor.get_component<scene::physics_spherical_joint_component>();

		float y_angle_limit = physics_spherical_joint_component.get_spherical_joint()->get_limit_cone().yAngle;
		float z_angle_limit = physics_spherical_joint_component.get_spherical_joint()->get_limit_cone().zAngle;

		if (editor_ui_utils::draw_property("Y Angle Limit", y_angle_limit, 0.0f, glm::pi<float>(), 0.01f))
		{
			physics_spherical_joint_component.get_spherical_joint()->set_limit_cone(y_angle_limit, z_angle_limit);
		}

		if (editor_ui_utils::draw_property("Z Angle Limit", z_angle_limit, 0.0f, glm::pi<float>(), 0.01f))
		{
			physics_spherical_joint_component.get_spherical_joint()->set_limit_cone(y_angle_limit, z_angle_limit);
		}
	}
}