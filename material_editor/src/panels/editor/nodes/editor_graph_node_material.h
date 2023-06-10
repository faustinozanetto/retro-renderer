#pragma once

#include "editor_graph_node.h"

#include <retro.h>

namespace retro::material_editor
{
    class editor_graph_node_material : public editor_graph_node
    {
    public:
        editor_graph_node_material(const std::shared_ptr<renderer::material>& value);
        ~editor_graph_node_material();

		/* Getters */
		const std::shared_ptr<renderer::material>& get_value() const { return m_value; }

        /* Functions */
        void on_draw_node() override;

    private:
        std::shared_ptr<renderer::material> m_value;
    };
}