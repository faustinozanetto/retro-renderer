#include "editor_actor_physics_material_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <utils/files.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_physics_material_component_panel::editor_actor_physics_material_component_panel() : editor_actor_component_panel("Physics Material Component")
	{
	}

	std::pair<bool, size_t> editor_actor_physics_material_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::physics_material_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::physics_material_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_physics_material_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &physics_material_component = current_scene->get_actors_registry()->get<scene::physics_material_component>(
			editor_main_layer::s_selected_actor);

		float static_friction = physics_material_component.get_physics_material()->get_static_friction();
		if (editor_ui_utils::draw_property("Static Friction", static_friction, 0.0f, 50.0f, 0.01f))
		{
			physics_material_component.get_physics_material()->set_static_friction(static_friction);
		}

		float dynamic_friction = physics_material_component.get_physics_material()->get_dynamic_friction();
		if (editor_ui_utils::draw_property("Dynamic Friction", dynamic_friction, 0.0f, 50.0f, 0.01f))
		{
			physics_material_component.get_physics_material()->set_dynamic_friction(dynamic_friction);
		}

		float restitution = physics_material_component.get_physics_material()->get_restitution();
		if (editor_ui_utils::draw_property("Restitution", restitution, 0.0f, 1.0f, 0.01f))
		{
			physics_material_component.get_physics_material()->set_restitution(restitution);
		}
	}
}