#include "editor_viewport_panel.h"

#include "../editor_app.h"

#include <core/application.h>

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
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        // Draw viewport
        ImGui::Image(
            reinterpret_cast<ImTextureID>(app->get_main_layer()->get_main_render_target()),
            ImVec2{viewportPanelSize.x, viewportPanelSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});
        ImGui::End();
    }
}