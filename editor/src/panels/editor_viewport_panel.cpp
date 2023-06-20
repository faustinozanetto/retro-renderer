#include "editor_viewport_panel.h"

#include "../editor_app.h"

#include <core/application.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace retro::editor
{
    editor_viewport_panel::editor_viewport_panel()
    {
    }

    editor_viewport_panel::~editor_viewport_panel() {}

    void editor_viewport_panel::on_render_panel()
    {
        RT_PROFILE;
        auto app = dynamic_cast<editor_app*>(&core::application::get());
        ImGui::Begin("Viewport");

		// Draw viewport
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image(
			reinterpret_cast<ImTextureID>(app->get_main_layer()->get_main_render_target()),
			ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });

        // ImGuizmo
		auto min_region = ImGui::GetWindowContentRegionMin();
		auto max_region = ImGui::GetWindowContentRegionMax();
		auto offset = ImGui::GetWindowPos();
        glm::vec2 bounds[2];
        bounds[0] = { min_region.x + offset.x, min_region.y + offset.y };
        bounds[1] = { max_region.x + offset.x, max_region.y + offset.y };

        ImGuizmo::SetRect(bounds[0].x, bounds[0].y, bounds[1].x - bounds[0].x, bounds[1].y - bounds[0].y);

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetOrthographic(false);
        app->get_main_layer()->draw_imguizmo();

        ImGui::End();
    }
}