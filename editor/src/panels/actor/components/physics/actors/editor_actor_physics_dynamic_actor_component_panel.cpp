#include "editor_actor_physics_dynamic_actor_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <utils/files.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_physics_dynamic_actor_component_panel::editor_actor_physics_dynamic_actor_component_panel() : editor_actor_component_panel("Physics Dynamic Actor Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_dynamic_actor_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::physics_dynamic_actor_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::physics_dynamic_actor_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_dynamic_actor_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &physics_dynamic_actor_component = current_scene->get_actors_registry()->get<scene::physics_dynamic_actor_component>(
			editor_main_layer::s_selected_actor);

		float mass = physics_dynamic_actor_component.get_dynamic_actor()->get_mass();
		if (editor_ui_utils::draw_property("Mass", mass, 0.01f, 100.0f, 0.01f)) {
			physics_dynamic_actor_component.get_dynamic_actor()->set_mass(mass);
		}

		float linear_damping = physics_dynamic_actor_component.get_dynamic_actor()->get_linear_damping();
		if (editor_ui_utils::draw_property("Linear Damping", linear_damping, 0.0f, 100.0f, 0.01f)) {
			physics_dynamic_actor_component.get_dynamic_actor()->set_linear_damping(linear_damping);
		}

		float angular_damping = physics_dynamic_actor_component.get_dynamic_actor()->get_angular_damping();
		if (editor_ui_utils::draw_property("Angular Damping", angular_damping, 0.0f, 100.0f, 0.01f)) {
			physics_dynamic_actor_component.get_dynamic_actor()->set_angular_damping(angular_damping);
		}
	}
}