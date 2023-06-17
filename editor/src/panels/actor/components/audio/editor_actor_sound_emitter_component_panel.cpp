#include "editor_actor_sound_emitter_component_panel.h"

#include <scene/scene_manager.h>

namespace retro::editor
{
	editor_actor_sound_emitter_component_panel::editor_actor_sound_emitter_component_panel() : editor_actor_component_panel("Sound Emitter Component")
	{
	}

	std::pair<bool, size_t> editor_actor_sound_emitter_component_panel::get_actor_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();
		bool has_component = current_scene->get_actors_registry()->any_of<scene::sound_emitter_component>(editor_main_layer::s_selected_actor);
		auto component_hash = typeid(scene::sound_emitter_component).hash_code();
		return std::make_pair(has_component, component_hash);
	}

	void editor_actor_sound_emitter_component_panel::on_render_component_details()
	{
		const auto current_scene = scene::scene_manager::get().get_active_scene();

		auto &sound_emitter_component = current_scene->get_actors_registry()->get<scene::sound_emitter_component>(
			editor_main_layer::s_selected_actor);

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

		}
		ImGui::SameLine();
		if (ImGui::Button("Pause")) {

		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {

		}
		ImGui::SameLine();
	}
}