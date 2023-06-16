#include "editor_actor_physics_d6_joint_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_physics_d6_joint_component_panel::editor_actor_physics_d6_joint_component_panel() : editor_actor_component_panel("Physics D6 Joint Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_d6_joint_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::physics_d6_joint_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::physics_d6_joint_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void EditMotion(physx::PxD6Axis::Enum axis, physx::PxD6Motion::Enum& type)
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		auto& physics_d6_joint_component = current_scene->get_actors_registry()->get<scene::physics_d6_joint_component>(
			editor_main_layer::s_selected_actor);

		ImGui::PushID(axis);

		const std::vector<std::string> motion_types = { "Locked", "Limited", "Free" };
		int selected_index = static_cast<int>(type);

		std::string name = "Motion " + std::string(physics::physics_utils::get_physx_d6_axis_to_string(axis));

		auto on_motion_selected = [&](int index) {
			type = static_cast<physx::PxD6Motion::Enum>(index);
			physics_d6_joint_component.get_d6_joint()->set_motion(axis, type);
			};

		editor_ui_utils::draw_combo_box(name, selected_index, motion_types, on_motion_selected);

		ImGui::PopID();
	}

	void editor_actor_physics_d6_joint_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &physics_d6_joint_component = current_scene->get_actors_registry()->get<scene::physics_d6_joint_component>(
			editor_main_layer::s_selected_actor);

		// Motion
		for (int axis = 0; axis < physx::PxD6Axis::eCOUNT; ++axis)
		{
			physx::PxD6Motion::Enum motion = physics_d6_joint_component.get_d6_joint()->get_motion(static_cast<physx::PxD6Axis::Enum>(axis));
			draw_d6_joint_motion_edit(physics_d6_joint_component.get_d6_joint(), static_cast<physx::PxD6Axis::Enum>(axis), motion);
		}

		// Distance Limit
		float distance_limit_extent = physics_d6_joint_component.get_d6_joint()->get_distance_limit().value;
		if (editor_ui_utils::draw_property("Distance Limit Extent", distance_limit_extent, 0.0f, 50.0f, 0.01f)) {
			physx::PxJointLinearLimit distance_limit = physx::PxJointLinearLimit(physics::physics_world::get().get_physics()->getTolerancesScale(), distance_limit_extent);
			physics_d6_joint_component.get_d6_joint()->set_distance_limit(distance_limit);
		}

		/*
		// Linear Limit, 3 because only the X,Y,Z axis.
		for (int axis = 0; axis < 3; ++axis)
		{
			ImGui::PushID(axis);
			physx::PxD6Axis::Enum axis_enum = static_cast<physx::PxD6Axis::Enum>(axis);
			auto axis_limit = physics_d6_joint_component.get_d6_joint()->get_linear_limit(axis_enum);
			float lower_limit = axis_limit.lower;
			float upper_limit = axis_limit.upper;

			if (editor_ui_utils::draw_property(std::format("{} Lower Limit", physics::physics_utils::get_physx_d6_axis_to_string(axis_enum)),
				lower_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
			{
				physics_d6_joint_component.get_d6_joint()->set_linear_limit(axis_enum, lower_limit, upper_limit);
			}

			if (editor_ui_utils::draw_property(std::format("{} Upper Limit", physics::physics_utils::get_physx_d6_axis_to_string(axis_enum)),
				upper_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
			{
				physics_d6_joint_component.get_d6_joint()->set_linear_limit(axis_enum, lower_limit, upper_limit);
			}

			ImGui::PopID();
		}
		*/
		float limit_lower_limit = physics_d6_joint_component.get_d6_joint()->get_twist_limit().lower;
		float limit_upper_limit = physics_d6_joint_component.get_d6_joint()->get_twist_limit().upper;

		if (editor_ui_utils::draw_property("Twist Lower Limit", limit_lower_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
		{
			physics_d6_joint_component.get_d6_joint()->set_twist_limit(limit_lower_limit, limit_upper_limit);
		}

		if (editor_ui_utils::draw_property("Twist Upper Limit", limit_upper_limit, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
		{
			physics_d6_joint_component.get_d6_joint()->set_twist_limit(limit_lower_limit, limit_upper_limit);
		}
	}

	void editor_actor_physics_d6_joint_component_panel::draw_d6_joint_motion_edit(const std::shared_ptr<physics::physics_d6_joint>& joint, physx::PxD6Axis::Enum axis, physx::PxD6Motion::Enum& type)
	{
		ImGui::PushID(axis);

		const std::vector<std::string> motion_types = { "Locked", "Limited", "Free" };
		int selected_index = static_cast<int>(type);

		std::string name = "Motion " + std::string(physics::physics_utils::get_physx_d6_axis_to_string(axis));

		auto on_motion_selected = [&](int index) {
			type = static_cast<physx::PxD6Motion::Enum>(index);
			joint->set_motion(axis, type);
		};

		editor_ui_utils::draw_combo_box(name, selected_index, motion_types, on_motion_selected);

		ImGui::PopID();
	}
}