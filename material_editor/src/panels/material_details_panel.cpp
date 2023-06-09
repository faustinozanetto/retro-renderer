#include "material_details_panel.h"

#include <core/application.h>
#include "../material_editor_app.h"

namespace retro::material_editor
{
    material_details_panel::material_details_panel()
    {
    }

    material_details_panel::~material_details_panel()
    {
    }

    void material_details_panel::on_render_panel()
    {
        auto& app = reinterpret_cast<material_editor_app&>(core::application::get());
        ImGui::Begin("Material Details");

        ImGui::Separator();
        ImGui::Text("Material File Path: %s", app.get_current_material_file_path().c_str());

        ImGui::End();
    }
}