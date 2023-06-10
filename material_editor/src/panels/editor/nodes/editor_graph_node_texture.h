#pragma once

#include "editor_graph_node.h"

#include <glm/glm.hpp>
#include <memory>
#include <retro.h>

namespace retro::material_editor
{
    class editor_graph_node_texture : public editor_graph_node
    {
    public:
        editor_graph_node_texture(const std::shared_ptr<renderer::texture>& texture = nullptr);
        ~editor_graph_node_texture();

        /* Getters */
        const std::shared_ptr<renderer::texture>& get_value() const { return m_value; }

        /* Functions */
        void on_draw_node() override;

    private:
        void load_texture_from_file();
        std::shared_ptr<renderer::texture> m_value;
    };
}