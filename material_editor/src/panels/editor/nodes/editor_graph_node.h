#pragma once

#include "imgui_node_editor.h"

#include <vector>
#include <string>
#include <functional>

namespace ImGuiNodeEditor = ax::NodeEditor;

namespace retro::material_editor
{
    enum class graph_node_pin_type
    {
        p_float,
        p_float2,
        p_float3,
        p_float4,
        p_color3,
        p_color4,
        p_texture,
    };

    enum class graph_node_pin_kind
    {
        input,
        output,
    };

    struct graph_node;

    struct graph_node_pin
    {
        ImGuiNodeEditor::PinId id;
        std::string name;
        graph_node *node;
        graph_node_pin_type type;
        graph_node_pin_kind kind;

        graph_node_pin(const std::string &name, int id, graph_node_pin_type pin_type) : name(name),
                                                                                        id(id), type(pin_type), kind(graph_node_pin_kind::input), node(nullptr) {}
    };

    struct graph_node
    {
        ImGuiNodeEditor::NodeId id;
        std::string name;
        std::vector<graph_node_pin> inputs;
        std::vector<graph_node_pin> outputs;

        graph_node() = default;

        graph_node(const std::string &name, int id) : name(name), id(id) {}
    };

    struct graph_node_link
    {
        ImGuiNodeEditor::LinkId id;

        ImGuiNodeEditor::PinId start_id;
        ImGuiNodeEditor::PinId end_id;

        graph_node_link(ImGuiNodeEditor::LinkId id, ImGuiNodeEditor::PinId start_id, ImGuiNodeEditor::PinId end_id) : id(id), start_id(start_id), end_id(end_id) {}
    };

    class editor_graph_node
    {
    public:
		using graph_node_value_updated = std::function<void(editor_graph_node*)>;

        editor_graph_node();
        ~editor_graph_node();

        /* Getters */
        graph_node *get_graph_node() { return m_graph_node; }

        /* Functions */
		void set_value_updated_callback(const graph_node_value_updated& callback) { m_value_updated_callback = callback; }

        virtual void on_draw_node() = 0;
        void on_value_updated();

    protected:
        graph_node* m_graph_node;
        graph_node_value_updated m_value_updated_callback;
    };
}