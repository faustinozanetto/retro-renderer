#include "editor_toolbar_panel.h"

#include "../editor_app.h"

#include <core/application.h>

namespace retro::editor
{
    editor_toolbar_panel::editor_toolbar_panel()
    {
    }

    editor_toolbar_panel::~editor_toolbar_panel() {}

    void editor_toolbar_panel::on_render_panel()
    {
        RT_PROFILE;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}