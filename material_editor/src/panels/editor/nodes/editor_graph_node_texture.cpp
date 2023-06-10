#include "editor_graph_node_texture.h"
#include "../editor_graph_panel.h"

#include "utils/files.h"

#include <retro.h>

namespace retro::material_editor
{
    editor_graph_node_texture::editor_graph_node_texture(const std::shared_ptr<renderer::texture>& texture) : editor_graph_node()
    {
        graph_node *node =new graph_node("Texture Node", editor_graph_panel::get_next_id());

        graph_node_pin output_pin = graph_node_pin("Texture Node", editor_graph_panel::get_next_id(), graph_node_pin_type::p_texture);

        output_pin.node = node;
        output_pin.kind = graph_node_pin_kind::output;
        node->outputs.push_back(output_pin);
        m_graph_node = node;
        m_value = texture;
    }

    editor_graph_node_texture::~editor_graph_node_texture()
    {
    }

    void editor_graph_node_texture::on_draw_node()
    {
        if (m_value) {
            if (ImGui::ImageButton((ImTextureID)(m_value->get_handle_id()), { 128,128 }, { 0,1 }, { 1,0 })) {
                load_texture_from_file();
            }
            ImGui::SameLine();
            ImGui::Text("Dimensions: %dx%d", m_value->get_data().width, m_value->get_data().height);
            ImGui::SameLine();
            ImGui::Text("Channels: %d", m_value->get_data().channels);
        }
        else {
            ImGui::Text("No Texture!");
            if (ImGui::Button("Load One")) {
                load_texture_from_file();
            }
        }
    }

    void editor_graph_node_texture::load_texture_from_file()
    {
        std::string file_path = files::open_file_dialog("Open Texture", { "Texture (*.png)", "*.png", "*.jpg" });
        if (!file_path.empty()) {
            m_value = renderer::texture_loader::load_texture_from_file(file_path);
            on_value_updated();
        }
    }
}