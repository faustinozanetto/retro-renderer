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
        RT_PROFILE_SECTION("mesh::construct_buffers");
        size_t vertex_buffer_size = m_vertices.size() * sizeof(mesh_vertex);
        size_t index_buffer_size = m_indices.size() * sizeof(mesh_index);

        m_vao = std::make_shared<vertex_array_object>();
        std::shared_ptr<vertex_buffer_object> vertices_vbo = std::make_shared<
            vertex_buffer_object>(vertex_buffer_object_target::arrays);

        std::shared_ptr<vertex_buffer_object> index_buffer = std::make_shared<
            vertex_buffer_object>(vertex_buffer_object_target::elements, m_indices.size());

        m_vao->bind();
        vertices_vbo->bind();
        vertices_vbo->set_data(vertex_buffer_object_usage::static_draw, vertex_buffer_size, m_vertices.data());

        index_buffer->bind();
        index_buffer->set_data(vertex_buffer_object_usage::static_draw, index_buffer_size, m_indices.data());

        std::initializer_list<vertex_buffer_layout_entry>
            layout_elements = {
                {"a_pos", vertex_buffer_entry_type::vec_float3, false},
                {"a_tex_coord", vertex_buffer_entry_type::vec_float2, false},
                {"a_normal", vertex_buffer_entry_type::vec_float3, false},
                {"a_tangent", vertex_buffer_entry_type::vec_float3, false},
                {"a_bitangent", vertex_buffer_entry_type::vec_float3, false}};

        const std::shared_ptr<vertex_buffer_layout_descriptor> vertices_vbo_layout_descriptor = std::make_shared<
            vertex_buffer_layout_descriptor>(layout_elements);
        vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

        m_vao->add_vertex_buffer(vertices_vbo);
        m_vao->set_index_buffer(index_buffer);
        m_vao->un_bind();
    }
}