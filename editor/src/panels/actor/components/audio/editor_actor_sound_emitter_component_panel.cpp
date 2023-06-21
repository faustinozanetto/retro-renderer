#include "editor_actor_sound_emitter_component_panel.h"

namespace retro::editor
{
	editor_actor_sound_emitter_component_panel::editor_actor_sound_emitter_component_panel() : editor_actor_component_panel("Sound Emitter Component")
	{
	}

	std::pair<bool, size_t> editor_actor_sound_emitter_component_panel::get_actor_component_details()
	{
		RT_PROFILE;
		bool has_component = editor_main_layer::s_selected_actor.has_component<scene::sound_emitter_component>();
		auto component_hash = typeid(scene::sound_emitter_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_sound_emitter_component_panel::on_render_component_details()
	{
		RT_PROFILE;
		const auto &sound_emitter_component = editor_main_layer::s_selected_actor.get_component<scene::sound_emitter_component>();

		float volume = sound_emitter_component.get_sound_emitter()->get_volume();
		if (editor_ui_utils::draw_property("Volume", volume, 0.0f, 5.0f, 0.01f)) {
			sound_emitter_component.get_sound_emitter()->set_volume(volume);
		}

		float pitch = sound_emitter_component.get_sound_emitter()->get_pitch();
		if (editor_ui_utils::draw_property("Pitch", pitch, 0.5f, 2.0f, 0.01f)) {
			sound_emitter_component.get_sound_emitter()->set_pitch(pitch);
		}

		float range = sound_emitter_component.get_sound_emitter()->get_range();
		if (editor_ui_utils::draw_property("Range", range, 0.0f, 10.0f, 0.01f)) {
			sound_emitter_component.get_sound_emitter()->set_range(range);
		}

		bool is_loop = sound_emitter_component.get_sound_emitter()->get_is_loop();
		if (editor_ui_utils::draw_property("Loop", is_loop)) {
			sound_emitter_component.get_sound_emitter()->set_is_loop(is_loop);
		}

		// Sound controls
		if (ImGui::Button("Play")) {
			sound_emitter_component.get_sound_emitter()->play();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause")) {
			sound_emitter_component.get_sound_emitter()->pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			sound_emitter_component.get_sound_emitter()->stop();
		}
		ImGui::SameLine();
	}

	void editor_actor_sound_emitter_component_panel::on_remove_component()
	{
		editor_main_layer::s_selected_actor.remove_component<scene::sound_emitter_component>();
	}
}
