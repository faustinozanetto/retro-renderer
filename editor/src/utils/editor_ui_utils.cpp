#include "editor_ui_utils.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace retro::editor
{
    void editor_ui_utils::draw_tooltip(const char* text)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
        if(ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(text);
            ImGui::EndTooltip();
        }
        ImGui::PopStyleVar();
    }

    void editor_ui_utils::draw_property(const std::string& name)
    {
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
    }

    void editor_ui_utils::draw_property(const std::string& name, const std::string& content)
    {
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        ImGui::Text(content.c_str());

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool editor_ui_utils::draw_property(const std::string& name, bool& value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::Checkbox(id.c_str(), &value))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, int& value, int min, int max, int reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::SliderInt(id.c_str(), &value, min, max))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, float& value, float min, float max, float step,
                                        float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::DragFloat(id.c_str(), &value, step, min, max))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, glm::vec2& value, float min, float max,
                                        float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
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

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, glm::vec3& value, float min, float max,
                                        float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
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

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, glm::vec3& value, bool color)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, glm::vec4& value, bool color)
    {
        bool modified = false;
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool editor_ui_utils::draw_property(const std::string& name, const std::shared_ptr<renderer::texture>& texture,
                                        const ImVec2& image_size, bool flip_image)
    {
        bool modified = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();

        ImGui::AlignTextToFramePadding();
        // Texture widget
        if (texture)
        {
            // Main image widget
            if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture->get_handle_id()), image_size,
                                   ImVec2(0.0f, flip_image ? 1.0f : 0.0f), ImVec2(1.0f, flip_image ? 0.0f : 1.0f)))
            {
                modified = true;
            }
            // Hover image widget
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Image(
                    reinterpret_cast<ImTextureID>(texture->get_handle_id()),
                    {256.0f, 256.0f},
                    ImVec2(0.0f, flip_image ? 1.0f : 0.0f), ImVec2(1.0f, flip_image ? 0.0f : 1.0f));
                ImGui::EndTooltip();
            }
        }
        else
        {
            // No Image
            if (ImGui::Button("Empty", image_size))
            {
                modified = true;
            }
        }

        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(texture ? texture->get_metadata().file_path.c_str() : "No Texture");
        // Texture details
        if (texture)
        {
            draw_tooltip(texture->get_metadata().file_path.c_str());
            ImGui::Text("Size %dx%d", texture->get_data().width, texture->get_data().height);
            ImGui::Text("Mip Maps: %d", texture->get_data().mip_map_levels);
            ImGui::Text("Channels: %d", texture->get_data().channels);
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();


        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::PopStyleVar();


        return modified;
    }

    void editor_ui_utils::draw_combo_box(const std::string& name, int& selection_index,
                                         const std::vector<std::string>& items, std::function<void(int)> on_selected)
    {
        if (ImGui::BeginCombo(name.c_str(), items[selection_index].c_str()))
        {
            for (int i = 0; i < items.size(); ++i)
            {
                bool is_selected = (selection_index == i);
                if (ImGui::Selectable(items[i].c_str(), is_selected))
                {
                    selection_index = i;
                    on_selected(i); // Call the provided callback function
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
}
