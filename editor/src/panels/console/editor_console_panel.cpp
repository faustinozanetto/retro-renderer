#include "editor_console_panel.h"

#include "../../editor_app.h"
#include "../../utils/editor_ui_utils.h"

#include <core/application.h>

namespace retro::editor
{
    editor_console_panel::editor_console_panel()
    {
    }

    editor_console_panel::~editor_console_panel() {}

    void editor_console_panel::on_render_panel()
    {
        auto app = dynamic_cast<editor_app *>(&core::application::get());
        auto console_panel = app->get_main_layer()->get_console_panel();
        auto camera = app->get_main_layer()->get_camera();

        ImGuiWindowFlags console_flags = ImGuiWindowFlags_NoCollapse;

        static bool console_open = true;
        ImGui::Begin("Console", &console_open, console_flags);
     //   ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
        for (const auto& message : m_messages) {
            ImGui::Text(message->get_message().c_str());
        }
       // ImGui::EndChild();
        ImGui::End();
    }

    void editor_console_panel::add_message(const std::shared_ptr<editor_console_message>& message)
    {
        m_messages.push_back(message);
    }
}