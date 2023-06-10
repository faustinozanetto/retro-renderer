#pragma once

#include "editor_graph_node.h"

#include <glm/glm.hpp>

namespace retro::material_editor
{
    class editor_graph_node_color3 : public editor_graph_node
    {
    public:
        editor_graph_node_color3(const glm::vec3& value = glm::vec3(0.0f));
        ~editor_graph_node_color3();

        /* Getters */
        const glm::vec3& get_value() const { return m_value; }

        /* Functions */
        void on_draw_node() override;

    private:
        glm::vec3 m_value;
    };
}