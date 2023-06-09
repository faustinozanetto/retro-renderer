#include "rtpch.h"
#include "model.h"

#include "model_loader.h"

namespace retro::renderer
{
    model::model(const std::string& file_name, const std::vector<std::shared_ptr<mesh>>& meshes) : asset({
        assets::asset_type::model, file_name
    })
    {
        m_meshes = meshes;
        RT_TRACE("Model Information:");
        RT_TRACE("  - Mesh Count: {}", m_meshes.size());
    }

    void model::serialize(std::ofstream& asset_pack_file)
    {
        RT_PROFILE;
        // Serialize the model's mesh count
        const uint32_t mesh_count = m_meshes.size();
        asset_pack_file.write(reinterpret_cast<const char*>(&mesh_count), sizeof(mesh_count));

        // Serialize each mesh
        for (const auto& mesh : m_meshes)
        {
            // Serialize the number of vertices
            const uint32_t vertices_count = mesh->get_vertices().size();
            asset_pack_file.write(reinterpret_cast<const char*>(&vertices_count), sizeof(vertices_count));

            // Serialize the vertices
            asset_pack_file.write(reinterpret_cast<const char*>(mesh->get_vertices().data()),
                                  vertices_count * sizeof(mesh_vertex));

            // Serialize the number of indices
            const uint32_t indices_count = mesh->get_indices().size();
            asset_pack_file.write(reinterpret_cast<const char*>(&indices_count), sizeof(indices_count));

            // Serialize the indices
            asset_pack_file.write(reinterpret_cast<const char*>(mesh->get_indices().data()),
                                  indices_count * sizeof(mesh_index));
        }
    }

    std::shared_ptr<model> model::deserialize(const assets::asset_metadata& metadata, std::ifstream& asset_pack_file)
    {
        RT_PROFILE;
        // Deserialize the model's mesh count
        uint32_t mesh_count;
        asset_pack_file.read(reinterpret_cast<char*>(&mesh_count), sizeof(mesh_count));

        // Deserialize each mesh
        std::vector<std::shared_ptr<mesh>> meshes;
        for (uint32_t i = 0; i < mesh_count; ++i)
        {
            // Read the number of vertices
            uint32_t vertices_count;
            asset_pack_file.read(reinterpret_cast<char*>(&vertices_count), sizeof(vertices_count));

            // Read the vertices
            std::vector<mesh_vertex> vertices(vertices_count);
            asset_pack_file.read(reinterpret_cast<char*>(vertices.data()), vertices_count * sizeof(mesh_vertex));

            // Read the number of indices
            uint32_t indices_count;
            asset_pack_file.read(reinterpret_cast<char*>(&indices_count), sizeof(indices_count));

            // Read the indices
            std::vector<mesh_index> indices(indices_count);
            asset_pack_file.read(reinterpret_cast<char*>(indices.data()), indices_count * sizeof(mesh_index));

            // Create a new mesh instance
            meshes.push_back(std::make_shared<mesh>(vertices, indices));
        }

        // Create a new model instance
        const std::shared_ptr<model>& model = model_loader::load_model_from_meshes(meshes);
        model->set_metadata(metadata);
        return model;
    }
}
