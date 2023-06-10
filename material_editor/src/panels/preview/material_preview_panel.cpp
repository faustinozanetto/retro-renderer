#include "material_preview_panel.h"

#include <core/application.h>
#include "../../material_editor_app.h"

namespace retro::material_editor
{
    material_preview_panel::material_preview_panel()
    {
    }

    material_preview_panel::~material_preview_panel()
    {
    }

    void material_preview_panel::on_render_panel()
    {
        const auto &app = dynamic_cast<material_editor_app*>(&core::application::get());
        ImGui::Begin("Material Preview");

        ImGui::Image((ImTextureID)(app->get_material_preview()->get_final_render_target()), { 512,512 }, { 0,1 }, { 1,0 });

        ImGui::End();
    }
}