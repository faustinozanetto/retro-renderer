#include "editor_actor_model_renderer_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <utils/files.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_model_renderer_component_panel::editor_actor_model_renderer_component_panel() : editor_actor_component_panel("Model Renderer Component")
	{
	}

	std::pair<bool, size_t> editor_actor_model_renderer_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::model_renderer_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::model_renderer_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_model_renderer_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &model_renderer_component = current_scene->get_actors_registry()->get<scene::model_renderer_component>(
			editor_main_layer::s_selected_actor);

		editor_ui_utils::draw_property("File Path", model_renderer_component.get_model()->get_metadata().file_path);
		if (ImGui::Button("Load Model")) {
			std::string file_path = files::open_file_dialog("Model", {"*.fbx", "*.obj", "*.gltf"});
			if (!file_path.empty()) {
				const auto& new_model = renderer::model_loader::load_model_from_file(file_path);
				model_renderer_component.set_model(new_model);
			}
		}
		editor_ui_utils::draw_property("Mesh Count", std::format("{}", model_renderer_component.get_model()->get_meshes().size()));
	}
}