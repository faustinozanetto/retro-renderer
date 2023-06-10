#include "editor_graph_node_float.h"

#include "../editor_graph_panel.h"
#include "../../../material_editor_app.h"

namespace retro::material_editor
{
    editor_graph_node_float::editor_graph_node_float() : editor_graph_node()
    {
        graph_node node = graph_node("Float Node", editor_graph_panel::get_next_id());

        graph_node_pin output_pin = graph_node_pin("Float", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);

        output_pin.node = &node;
        output_pin.kind = graph_node_pin_kind::output;
        node.outputs.push_back(output_pin);
        m_graph_node = node;
        m_value = 0.0f;
    }

    editor_graph_node_float::~editor_graph_node_float()
    {
    }

    void editor_graph_node_float::on_draw_node()
    {
        ImGui::SliderFloat("Value", &m_value, 0.0f, 10.0f);
    }

    void editor_graph_node_float::on_value_updated()
    {
    }
}