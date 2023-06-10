#pragma once

#include "editor_graph_node.h"

namespace retro::material_editor
{
    class editor_graph_node_float : public editor_graph_node
    {
    public:
        editor_graph_node_float(float value = 0.0f, float min = 0.0f, float max = 1.0f);
        ~editor_graph_node_float();

		/* Getters */
		float get_value() const { return m_value; }

		/* Functions */
		void on_draw_node() override;

    private:
        float m_value;
        float m_min;
        float m_max;
    };
}