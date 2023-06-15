#include "editor_ui_utils.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace retro::editor
{
    void editor_ui_utils::draw_property(const std::string &name)
    {
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
    }

    void editor_ui_utils::draw_property(const std::string &name, const std::string &content)
    {
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        ImGui::Text(content.c_str());

        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool editor_ui_utils::draw_property(const std::string &name, bool &value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        const std::string id = "##" + name;
        if (ImGui::Checkbox(id.c_str(), &value))
        {
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, int &value, int min, int max, int reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        const std::string id = "##" + name;
        if (ImGui::SliderInt(id.c_str(), &value, min, max))
        {
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, float &value, float min, float max, float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        const std::string id = "##" + name;
        if (ImGui::DragFloat(id.c_str(), &value, step, min, max))
        {
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, glm::vec2 &value, float min, float max,
                                        float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
        {
            value.x = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f"))
            modified = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
        {
            value.y = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f"))
            modified = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, glm::vec3 &value, float min, float max,
                                        float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
        {
            value.x = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
        {
            value.y = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
        {
            value.z = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Z", &value.z, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, glm::vec3 &value, bool color)
    {
        bool modified = false;
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const std::string id = "##" + name;
        if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string &name, glm::vec4 &value, bool color)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        const std::string id = "##" + name;
        if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }
}