#include "editor_graph_node_material.h"
#include "../editor_graph_panel.h"

#include <retro.h>

namespace retro::material_editor
{
    editor_graph_node_material::editor_graph_node_material() : editor_graph_node()
    {
        graph_node node = graph_node("Material Output", editor_graph_panel::get_next_id());
        // Parameters inputs
        // Albedo
        {
            graph_node_pin albedo_input_pin = graph_node_pin("Albedo", editor_graph_panel::get_next_id(), graph_node_pin_type::p_color3);
            albedo_input_pin.node = &node;
            albedo_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(albedo_input_pin);
        }
        // Emissive
        {
            graph_node_pin emissive_input_pin = graph_node_pin("Emissive", editor_graph_panel::get_next_id(), graph_node_pin_type::p_color3);
            emissive_input_pin.node = &node;
            emissive_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(emissive_input_pin);
        }
        // Roughness
        {
            graph_node_pin roughness_input_pin = graph_node_pin("Roughness", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);
            roughness_input_pin.node = &node;
            roughness_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(roughness_input_pin);
        }
        // Metallic
        {
            graph_node_pin metallic_input_pin = graph_node_pin("Metallic", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);
            metallic_input_pin.node = &node;
            metallic_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(metallic_input_pin);
        }
        // Ambient Occlusion
        {
            graph_node_pin ambient_occlusion_input_pin = graph_node_pin("Ambient Occlusion", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);
            ambient_occlusion_input_pin.node = &node;
            ambient_occlusion_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(ambient_occlusion_input_pin);
        }
        // Emissive Strength
        {
            graph_node_pin emissive_strength_input_pin = graph_node_pin("Emissive Strength", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);
            emissive_strength_input_pin.node = &node;
            emissive_strength_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(emissive_strength_input_pin);
        }
        // Texture Tilling
        {
            graph_node_pin texture_tilling_input_pin = graph_node_pin("Texture Tilling", editor_graph_panel::get_next_id(), graph_node_pin_type::p_float);
            texture_tilling_input_pin.node = &node;
            texture_tilling_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(texture_tilling_input_pin);
        }

        // Textures input
        for (auto texture_type : renderer::material::get_material_texture_types())
        {
            graph_node_pin texture_input_pin = graph_node_pin(renderer::material::get_material_texture_type_to_string(texture_type),
                                                               editor_graph_panel::get_next_id(), graph_node_pin_type::p_texture);

            texture_input_pin.node = &node;
            texture_input_pin.kind = graph_node_pin_kind::input;
            node.inputs.push_back(texture_input_pin);
        }
        m_graph_node = node;
    }

    editor_graph_node_material::~editor_graph_node_material()
    {
    }

    void editor_graph_node_material::on_draw_node()
    {
    }

    void editor_graph_node_material::on_value_updated()
    {
    }
}