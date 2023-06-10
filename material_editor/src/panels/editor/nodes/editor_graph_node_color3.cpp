#include "editor_graph_node_color3.h"

#include "../editor_graph_panel.h"
#include <glm/gtc/type_ptr.hpp>

namespace retro::material_editor
{
    editor_graph_node_color3::editor_graph_node_color3()
    {
        graph_node* node = new graph_node("Color 3 Node", editor_graph_panel::get_next_id());

        graph_node_pin output_pin = graph_node_pin("Color 3 Node", editor_graph_panel::get_next_id(), graph_node_pin_type::p_color3);

        output_pin.node = node;
        output_pin.kind = graph_node_pin_kind::output;
        node->outputs.push_back(output_pin);

        m_graph_node = node;
        m_value = glm::vec3(0.0f);
    }

    editor_graph_node_color3::~editor_graph_node_color3()
    {
    }

    void editor_graph_node_color3::on_draw_node()
    {
        if (ImGui::ColorPicker3("Value", glm::value_ptr(m_value), ImGuiColorEditFlags_NoTooltip)) {
            on_value_updated();
        }
    }
}