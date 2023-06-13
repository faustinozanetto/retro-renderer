#include "material_preview_panel.h"

#include <core/application.h>
#include "../../material_editor_app.h"
#include <utils/files.h>
#include <glm/gtc/type_ptr.hpp>

namespace retro::material_editor
{
    material_preview_panel::material_preview_panel()
    {
        m_rotate_model = false;
        setup_preview_models();
    }

    material_preview_panel::~material_preview_panel()
    {
    }

    void material_preview_panel::on_render_panel()
    {
        const auto &app = dynamic_cast<material_editor_app *>(&core::application::get());
        ImGui::Begin("Customize Preview");

        auto &transform_component = app->get_material_preview()->get_preview_actor()->get_component<scene::transform_component>();

        if (m_rotate_model)
        {
            glm::quat rotation = transform_component.get_rotation();
            rotation.y += core::time::get_delta_time();
            transform_component.set_rotation(rotation);
        }

        glm::vec3 location = transform_component.get_location();
        if (ImGui::SliderFloat3("Location", glm::value_ptr(location), -10.0f, 10.0f))
        {
            transform_component.set_location(location);
        }

        glm::vec3 rotation = transform_component.get_rotation();
        if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -5.0f, 5.0f))
        {
            transform_component.set_rotation(rotation);
        }

        glm::vec3 scale = transform_component.get_scale();
        if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.0f, 10.0f))
        {
            transform_component.set_scale(scale);
        }

        // Rotation
        ImGui::Checkbox("Rotate Model", &m_rotate_model);
        ImGui::Text("Model Selection");
        for (material_previw_panel_model &preview_model : m_preview_models)
        {
            if (ImGui::Button(preview_model.label.c_str()))
            {
                auto &model = preview_model.model;
                if (preview_model.label == "Custom")
                {
                    std::string file_path = files::open_file_dialog("Model", {"*.fbx", "*.obj", "*.gltf"});
                    if (!file_path.empty())
                    {
                        model = renderer::model_loader::load_model_from_file(file_path);
                    }
                }
                app->get_material_preview()->get_preview_actor()->get_component<scene::model_renderer_component>().set_model(model);
            }
            ImGui::SameLine();
        }

        ImGui::End();

        ImGui::Begin("Material Geometry");
        ImVec2 image_size = ImGui::GetContentRegionAvail();
        ImGui::Columns(2);
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_geometry_fbo()->get_attachment_id(0)), image_size / 2, {0, 1}, {1, 0});
        ImGui::NextColumn();
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_geometry_fbo()->get_attachment_id(1)), image_size / 2, {0, 1}, {1, 0});
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_geometry_fbo()->get_attachment_id(2)), image_size / 2, {0, 1}, {1, 0});
        ImGui::NextColumn();
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_geometry_fbo()->get_attachment_id(3)), image_size / 2, {0, 1}, {1, 0});
        ImGui::Columns(1);

        ImGui::Image((ImTextureID)(app->get_material_preview()->get_geometry_fbo()->get_attachment_id(4)), image_size / 2, {0, 1}, {1, 0});

        ImGui::End();

        ImGui::Begin("Material Preview");
        image_size = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(app->get_material_preview()->get_final_render_target()), image_size, {0, 1}, {1, 0});
        ImGui::End();
    }

    void material_preview_panel::setup_preview_models()
    {
        material_previw_panel_model cube;
        cube.label = "Cube";
        cube.model = renderer::model_loader::load_model_from_file("resources/models/cube.obj");
        m_preview_models.push_back(cube);

        material_previw_panel_model sphere;
        sphere.label = "Sphere";
        sphere.model = renderer::model_loader::load_model_from_file("resources/models/sphere.obj");
        m_preview_models.push_back(sphere);

        material_previw_panel_model monkey;
        monkey.label = "Monkey";
        monkey.model = renderer::model_loader::load_model_from_file("resources/models/monkey.obj");
        m_preview_models.push_back(monkey);

        material_previw_panel_model custom;
        custom.label = "Custom";
        m_preview_models.push_back(custom);
    }
}