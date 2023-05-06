#pragma once

#include "renderer/buffers/vertex_array_object.h"

#include <glm/glm.hpp>

namespace retro::renderer
{
    typedef uint32_t mesh_index;

    struct mesh_vertex
    {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normals;
    };

    class mesh
    {
    public:
        mesh(const std::vector<mesh_vertex> &vertices, const std::vector<mesh_index> &indices);

        const std::shared_ptr<vertex_array_object> &get_vao() const { return m_vao; }

    private:
        void construct_buffers();

        std::shared_ptr<vertex_array_object> m_vao;
        std::vector<mesh_vertex> m_vertices;
        std::vector<mesh_index> m_indices;
    };
}