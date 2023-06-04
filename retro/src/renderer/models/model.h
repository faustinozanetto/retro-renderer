#pragma once

#include "assets/asset.h"
#include "renderer/models/mesh.h"

namespace retro::renderer
{
    class model : public assets::asset
    {
    public:
        model(const std::string &file_name, const std::vector<std::shared_ptr<mesh>> &meshes);

        /* Getters */
        const std::vector<std::shared_ptr<mesh>> &get_meshes() const { return m_meshes; }

        /* Asset */
        void serialize(std::ofstream &asset_pack_file) override;
        static std::shared_ptr<model> deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file);

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
    };
}