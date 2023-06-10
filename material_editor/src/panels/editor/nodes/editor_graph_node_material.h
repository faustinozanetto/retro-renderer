#pragma once

#include "editor_graph_node.h"

#include <retro.h>

namespace retro::material_editor
{
    class editor_graph_node_material : public editor_graph_node
    {
    public:
        editor_graph_node_material();
        ~editor_graph_node_material();

        void on_draw_node() override;

    private:
        std::shared_ptr<renderer::material> m_value;
    };
}