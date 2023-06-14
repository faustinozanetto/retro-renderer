#include "editor_viewport_panel.h"

namespace retro::editor
{
    editor_viewport_panel::editor_viewport_panel()
    {
    }

    editor_viewport_panel::~editor_viewport_panel() {}

    void editor_viewport_panel::on_render_panel()
    {
		ImGui::Begin("Viewport");
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(0),
			ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });
		ImGui::End();
    }
}