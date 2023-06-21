#include "editor_actor_sound_source_component_panel.h"

#include "audio/sound_loader.h"
#include "utils/files.h"

namespace retro::editor
{
	editor_actor_sound_source_component_panel::editor_actor_sound_source_component_panel() : editor_actor_component_panel("Sound Source Component")
	{
	}

	std::pair<bool, size_t> editor_actor_sound_source_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::sound_source_component>();
		auto component_hash = typeid(scene::sound_source_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_sound_source_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		auto& sound_source_component = editor_main_layer::s_selected_actor.get_component<scene::sound_source_component>();

		if (sound_source_component.get_sound())
		{
			ImGui::Text("File Path: %s", sound_source_component.get_sound()->get_metadata().file_path.c_str());
			ImGui::Text("Frequency: %.4f", sound_source_component.get_sound()->get_frequency());
			ImGui::Text("Length: %.4f", sound_source_component.get_sound()->get_length() / 1000.0f);
			ImGui::Text("Channels: %d", sound_source_component.get_sound()->get_channels());
		} else
		{
			ImGui::Text("No Sound!");
		}

		if (ImGui::Button("Load Sound")) {
			const std::string file_path = files::open_file_dialog("Sound File", { "*.ogg", "*.wav", "*.mp3" });
			if (!file_path.empty()) {
				const auto& loaded_sound = audio::sound_loader::load_sound_from_file(file_path);
				sound_source_component.set_sound(loaded_sound);
				// Update sound in the actor sound emitter if any.
				if (editor_main_layer::s_selected_actor.has_component<scene::sound_emitter_component>()) {
					const auto& sound_emitter_component = editor_main_layer::s_selected_actor.get_component<scene::sound_emitter_component>();
					sound_emitter_component.get_sound_emitter()->set_sound(loaded_sound);
				}
			}
		}
	}

	void editor_actor_sound_source_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::sound_source_component>();
	}
}
