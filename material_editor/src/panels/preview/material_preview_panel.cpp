#include "material_preview_panel.h"

#include <core/application.h>
#include "../../material_editor_app.h"
#include <glm/gtc/type_ptr.hpp>

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
        ImGui::Begin("Customize Preview");

		auto& transform_component = app->get_material_preview()->get_preview_actor()->get_component<scene::transform_component>();

		glm::vec3 location = transform_component.get_location();
		if (ImGui::SliderFloat3("Location", glm::value_ptr(location), -10.0f, 10.0f)) {
			transform_component.set_location(location);
		}

		glm::vec3 rotation = transform_component.get_rotation();
		if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -5.0f, 5.0f)) {
			transform_component.set_rotation(rotation);
		}

		glm::vec3 scale = transform_component.get_scale();
		if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.0f, 10.0f)) {
			transform_component.set_scale(scale);
		}

        ImGui::End();

        ImGui::Begin("Material Preview");

        ImVec2 image_size = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_final_render_target()), image_size, { 0,1 }, { 1,0 });

        ImGui::End();
    }
}