#include "editor_toolbar_panel.h"

#include "../editor_app.h"

#include <core/application.h>

namespace retro::editor
{
    editor_toolbar_panel::editor_toolbar_panel() : editor_panel("Toolbar")
    {
    }

    editor_toolbar_panel::~editor_toolbar_panel()
    {
    }

    void editor_toolbar_panel::on_render_panel()
    {
        RT_PROFILE;
        auto app = dynamic_cast<editor_app*>(&core::application::get());
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Project"))
                {
                }

                if (ImGui::MenuItem("New Scene", "CTRL+N"))
                {
                }

                if (ImGui::MenuItem("Save Scene", "CTRL+S"))
                {
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Panels"))
            {
                for (auto& editor_panel : app->get_main_layer()->get_editor_panels())
                {
                    if (editor_panel->get_name() == "Toolbar") continue;

                    if (ImGui::MenuItem(editor_panel->get_name().c_str(), "", &editor_panel->get_show_panel(), true))
                    {
                        editor_panel->set_show_panel(true);
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Actor"))
            {
                auto current_scene = scene::scene_manager::get().get_active_scene();
                if (ImGui::MenuItem("Create Empty"))
                {
                    current_scene->create_actor("Empty Actor");
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}
