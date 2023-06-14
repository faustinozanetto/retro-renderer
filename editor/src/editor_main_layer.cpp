#include "editor_main_layer.h"

#include "ui/engine_ui.h"

#include "panels/editor_viewport_panel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace retro::editor
{
    editor_main_layer::editor_main_layer()
    {
        initialize();
    }

    editor_main_layer::~editor_main_layer()
    {
    }

    void editor_main_layer::initialize()
    {
        // Create editor panels
        m_panels.push_back(std::make_shared<editor_viewport_panel>());
    }

    void editor_main_layer::on_update()
    {
        ui::engine_ui::begin_frame();
        begin_dockspace();
        // Render panels
        for (const auto& panel : m_panels) {
            panel->on_render_panel();
        }
        end_dockspace();
        ui::engine_ui::end_frame();
    }

    void editor_main_layer::begin_dockspace()
    {
        static bool open = true;
        static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        const float toolbar_height = ImGui::GetFrameHeight();
        ImGui::SetNextWindowPos({viewport->Pos.x, viewport->Pos.y + toolbar_height});
        ImGui::SetNextWindowSize({viewport->Size.x, viewport->Size.y - toolbar_height});
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockMain = ImGui::GetID("MyDockspace");
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::DockSpace(dockMain, ImVec2(0.0f, 0.0f), opt_flags);
        }
    }

    void editor_main_layer::end_dockspace()
    {
        ImGui::End();
    }
}