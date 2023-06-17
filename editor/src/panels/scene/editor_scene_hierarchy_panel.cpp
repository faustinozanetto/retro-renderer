#include "editor_scene_hierarchy_panel.h"

#include "../../editor_app.h"
#include "../../utils/editor_ui_utils.h"
#include "../../editor_main_layer.h"

#include <core/application.h>
#include <scene/scene_manager.h>

namespace retro::editor
{
	editor_scene_hierarchy_panel::editor_scene_hierarchy_panel()
	{
	}

	editor_scene_hierarchy_panel::~editor_scene_hierarchy_panel() {}

	void editor_scene_hierarchy_panel::on_render_panel()
	{
		RT_PROFILE;
		auto app = dynamic_cast<editor_app *>(&core::application::get());
		auto current_scene = scene::scene_manager::get().get_active_scene();

		ImGui::Begin("Hierarchy");
		if (current_scene)
		{
			current_scene->get_actors_registry()->each([&](entt::entity actor_handle) {
				
				const auto& name_component = current_scene->get_actors_registry()->get<scene::name_component>(actor_handle);

				ImGuiTreeNodeFlags flags = ((editor_main_layer::s_selected_actor == actor_handle) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)actor_handle, flags, name_component.get_name().c_str());

				// Selection of actor
				if (ImGui::IsItemClicked())
				{
					editor_main_layer::s_selected_actor = actor_handle;
				}

				if (opened)
				{
					ImGui::TreePop();
				}
			});
		}
		else {
			ImGui::Text("No active scene!");
		}
		ImGui::End();
	}
}