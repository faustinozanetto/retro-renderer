#pragma once

#include "editor_graph_node.h"

#include <glm/glm.hpp>

namespace retro::material_editor
{
    class editor_graph_node_color3 : public editor_graph_node
    {
    public:
        editor_graph_node_color3();
        ~editor_graph_node_color3();

        void on_draw_node() override;

    private:
        glm::vec3 m_value;
    };
}