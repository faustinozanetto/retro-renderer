#include "editor_actor_model_renderer_component_panel.h"

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
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::model_renderer_component>();
		auto component_hash = typeid(scene::model_renderer_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_model_renderer_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		auto &model_renderer_component = editor_main_layer::s_selected_actor.get_component<scene::model_renderer_component>();

		// Model details section
		if (model_renderer_component.get_model())
		{
			editor_ui_utils::draw_property("File Path", model_renderer_component.get_model()->get_metadata().file_path);
			editor_ui_utils::draw_property("Mesh Count", std::format("{}", model_renderer_component.get_model()->get_meshes().size()));
		} else
		{
			ImGui::Text("No Model!");
		}
		// Load model section
		if (ImGui::Button("Load Model")) {
			const std::string file_path = files::open_file_dialog("Model", {"*.fbx", "*.obj", "*.gltf"});
			if (!file_path.empty()) {
				const auto& new_model = renderer::model_loader::load_model_from_file(file_path);
				model_renderer_component.set_model(new_model);
			}
		}

	}
}