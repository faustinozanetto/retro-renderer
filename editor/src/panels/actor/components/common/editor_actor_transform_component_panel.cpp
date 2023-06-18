#include "editor_actor_transform_component_panel.h"

namespace retro::editor
{
	editor_actor_transform_component_panel::editor_actor_transform_component_panel() : editor_actor_component_panel("Transform Component")
	{
	}

	std::pair<bool, size_t> editor_actor_transform_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::transform_component>();
		auto component_hash = typeid(scene::transform_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}
	
	void editor_actor_transform_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto &transform_component = editor_main_layer::s_selected_actor.get_component<scene::transform_component>();

		auto& transform = transform_component.get_transform();

		glm::vec3 location = glm::vec3(transform->get_location());
		if (editor_ui_utils::draw_property("Position", location, -25.0f, 25.0f, 0.05f))
		{
			transform->set_location(location);
		}

		glm::vec3 rotation = glm::vec3(glm::eulerAngles(transform->get_rotation()));
		if (editor_ui_utils::draw_property("Rotation", rotation, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.05f))
		{
			glm::quat pitch = glm::angleAxis(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat yaw = glm::angleAxis(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat roll = glm::angleAxis(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			transform->set_rotation(roll * yaw * pitch);
		}

		glm::vec3 scale = glm::vec3(transform->get_scale());
		if (editor_ui_utils::draw_property("Scale", scale, -10.0f, 10.0f, 0.05f))
		{
			transform->set_scale(scale);
		}
	}
}