#pragma once

#include "editor_graph_node.h"

namespace retro::material_editor
{
    class editor_graph_node_float : public editor_graph_node
    {
    public:
        editor_graph_node_float();
        ~editor_graph_node_float();

        void on_draw_node() override;
        void on_value_updated() override;

    private:
        float m_value;
    };
}