#include "material_editor_app.h"

#include "panels/editor/editor_graph_panel.h"
#include "panels/material_details_panel.h"
#include "panels/preview/material_preview_panel.h"

#include <core/entry_point.h>
#include <utils/files.h>

#include <imgui.h>

namespace retro::material_editor
{
    material_editor_app::material_editor_app() : application("../", {"Retro Material Editor", 1920, 1080})
    {
        initialize();
    }

    material_editor_app::~material_editor_app()
    {
    }

    void material_editor_app::on_update()
    {
        ui::engine_ui::begin_frame();
        // Note: Switch this to true to enable dockspace
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        ImGui::Begin("Viewport");

        for (auto &editor_panel : m_editor_panels)
        {
            editor_panel->on_render_panel();
        }

        if (m_current_material)
            m_material_preview->render_preview();

        ImGui::End();
        ImGui::End();

        ui::engine_ui::end_frame();
    }

    void material_editor_app::on_open_material_file()
    {
        std::string file_path = files::open_file_dialog("Retro Renderer Material", {"Material Files (*.rrm)", "*.rrm"});
        if (!file_path.empty())
        {
            m_current_material = renderer::material_loader::load_material_from_file(file_path);
            m_material_preview->on_material_selected(m_current_material);
        }
    }

    void material_editor_app::initialize()
    {
        m_current_material = renderer::material_loader::load_material_from_file("resources/materials/radio.rrm");
        m_editor_panels.push_back(std::make_shared<editor_graph_panel>());
        m_editor_panels.push_back(std::make_shared<material_details_panel>());
        m_editor_panels.push_back(std::make_shared<material_preview_panel>());

        m_material_preview = std::make_shared<material_preview>();

        m_material_preview->on_material_selected(m_current_material);
    }

    void material_editor_app::on_handle_event(retro::events::base_event &event)
    {
    }

    bool material_editor_app::on_window_resize(retro::events::window_resize_event &resize_event)
    {
        return application::on_window_resize(resize_event);
    }
}

retro::core::application *retro::core::create_application()
{
    return new retro::material_editor::material_editor_app();
}