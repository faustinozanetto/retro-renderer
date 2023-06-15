#include "editor_actor_transform_component_panel.h"

#include <scene/scene_manager.h>

namespace retro::editor
{
	editor_actor_transform_component_panel::editor_actor_transform_component_panel() : editor_actor_component_panel("Transform Component")
	{
	}

	std::pair<bool, size_t> editor_actor_transform_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::transform_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::transform_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}
	
	void editor_actor_transform_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &transform_component = current_scene->get_actors_registry()->get<scene::transform_component>(
			editor_main_layer::s_selected_actor);

		glm::vec3 location = glm::vec3(transform_component.get_location());
		if (editor_ui_utils::draw_property("Position", location, -25.0f, 25.0f, 0.05f))
		{
			transform_component.set_location(location);
		}

		glm::vec3 rotation = glm::vec3(glm::eulerAngles(transform_component.get_rotation()));
		if (editor_ui_utils::draw_property("Rotation", rotation, -25.0f, 25.0f, 0.05f))
		{
			glm::quat pitch = glm::angleAxis(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat yaw = glm::angleAxis(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat roll = glm::angleAxis(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			transform_component.set_rotation(roll * yaw * pitch);
		}

		glm::vec3 scale = glm::vec3(transform_component.get_scale());
		if (editor_ui_utils::draw_property("Scale", scale, -10.0f, 10.0f, 0.05f))
		{
			transform_component.set_scale(scale);
		}
	}
}