#include "rtpch.h"
#include "mesh.h"

namespace retro::renderer
{
    mesh::mesh(const std::vector<mesh_vertex> &vertices, const std::vector<mesh_index> &indices)
    {
        m_vertices = vertices;
        m_indices = indices;

        construct_buffers();
    }

    void mesh::construct_buffers()
    {
        m_vao = std::make_shared<vertex_array_object>();
        m_vao->bind();

        // 1. Setup vertices buffer
        {
            size_t size = m_vertices.size() * sizeof(&m_vertices[0]);
            std::shared_ptr<retro::renderer::vertex_buffer_object> vertices_vbo = std::make_shared<
                retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::arrays);
            vertices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, m_vertices.data());

            std::initializer_list<retro::renderer::vertex_buffer_layout_entry>
                layout_elements = {
                    {"a_pos", retro::renderer::vertex_buffer_entry_type::vec_float3, false},
                    {"a_tex_coord", retro::renderer::vertex_buffer_entry_type::vec_float2, false}};

            std::shared_ptr<retro::renderer::vertex_buffer_layout_descriptor> vertices_vbo_layout_descriptor = std::make_shared<
                retro::renderer::vertex_buffer_layout_descriptor>(layout_elements);
            vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);
            m_vao->add_vertex_buffer(vertices_vbo);
        }

        // 2. Setup indices buffer
        {
            size_t size = m_indices.size() * sizeof(&m_indices[0]);
            std::shared_ptr<retro::renderer::vertex_buffer_object> indices_vbo = std::make_shared<
                retro::renderer::vertex_buffer_object>(retro::renderer::vertex_buffer_object_target::elements);
            indices_vbo->set_data(retro::renderer::vertex_buffer_object_usage::static_draw, size, m_indices.data());
        }

        m_vao->un_bind();
    }
}