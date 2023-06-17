#include "editor_actor_material_renderer_component_panel.h"

#include <scene/scene_manager.h>
#include <renderer/models/model_loader.h>
#include <utils/files.h>
#include <memory>
#include <format>

namespace retro::editor
{
	editor_actor_material_renderer_component_panel::editor_actor_material_renderer_component_panel() : editor_actor_component_panel("Material Renderer Component")
	{
	}

	std::pair<bool, size_t> editor_actor_material_renderer_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		const auto& current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::material_renderer_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::material_renderer_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_material_renderer_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto& current_scene = scene::scene_manager::get().get_active_scene();

		auto &material_renderer_component = current_scene->get_actors_registry()->get<scene::material_renderer_component>(
			editor_main_layer::s_selected_actor);

		editor_ui_utils::draw_property("Parameters");
		ImGui::Separator();
		glm::vec3 albedo = material_renderer_component.get_material()->get_albedo();
		if (editor_ui_utils::draw_property("Albedo", albedo, true)) {
			material_renderer_component.get_material()->set_albedo(albedo);
		}

		glm::vec3 emissive = material_renderer_component.get_material()->get_emissive();
		if (editor_ui_utils::draw_property("Emissive", emissive, true)) {
			material_renderer_component.get_material()->set_emissive(emissive);
		}

		float roughness = material_renderer_component.get_material()->get_roughness();
		if (editor_ui_utils::draw_property("Roughness", roughness, 0.0f, 1.0f, 0.01f)) {
			material_renderer_component.get_material()->set_roughness(roughness);
		}

		float metallic = material_renderer_component.get_material()->get_metallic();
		if (editor_ui_utils::draw_property("Metallic", metallic, 0.0f, 1.0f, 0.01f)) {
			material_renderer_component.get_material()->set_metallic(metallic);
		}

		float emissive_strength = material_renderer_component.get_material()->get_emissive_strength();
		if (editor_ui_utils::draw_property("Emissive Strength", emissive_strength, 0.0f, 25.0f, 0.01f)) {
			material_renderer_component.get_material()->set_emissive_strength(emissive_strength);
		}

		float ambient_occlusion = material_renderer_component.get_material()->get_ambient_occlusion();
		if (editor_ui_utils::draw_property("Ambient Occlusion", ambient_occlusion, 0.0f, 1.0f, 0.01f)) {
			material_renderer_component.get_material()->set_ambient_occlusion(ambient_occlusion);
		}

		float tilling = material_renderer_component.get_material()->get_tilling();
		if (editor_ui_utils::draw_property("Tilling", tilling, 0.0f, 10.0f, 0.01f)) {
			material_renderer_component.get_material()->set_tilling(tilling);
		}

		editor_ui_utils::draw_property("Textures");
		ImGui::Separator();
		const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
		for (const auto& [type, texture] : material_renderer_component.get_material()->get_material_textures()) {
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(type), texture_flags, renderer::material::get_material_texture_type_to_string(type))) {
				// Texture preview and load
				if (editor_ui_utils::draw_property("Texture", texture.texture)) {
					std::string file_path = files::open_file_dialog("Texture File", { "*.png", "*.jpg", "*.jpeg" });
					if (!file_path.empty()) {
						// Create and load new texture and save the material texture.
						auto new_texture = renderer::texture_loader::load_texture_from_file(file_path);
						renderer::material_texture material_texture;
						material_texture.is_enabled = true;
						material_texture.type = type;
						material_texture.texture = new_texture;
						material_renderer_component.get_material()->set_texture(material_texture);
					}
				}
				// Texture enable or disable
				bool texture_enabled = texture.is_enabled;
				if (editor_ui_utils::draw_property("Enabled", texture_enabled)) {
					material_renderer_component.get_material()->set_texture_enabled(type, texture_enabled);
				}
				ImGui::TreePop();
			}
		}
	}
}