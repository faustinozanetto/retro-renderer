#pragma once

#include "assets/asset.h"
#include "renderer/models/mesh.h"
#include "math/bounding_box.h"

namespace retro::renderer
{
    class model : public assets::asset
    {
    public:
        model(const std::string &file_name, const std::vector<std::shared_ptr<mesh>> &meshes);

        /* Getters */
        const std::vector<std::shared_ptr<mesh>> &get_meshes() const { return m_meshes; }
        const math::bounding_box& get_bounding_box() const { return m_bounding_box; }

        /* Setters */
        void set_bounding_box(const math::bounding_box& bounding_box) { m_bounding_box = bounding_box; }

        /* Asset */
        void serialize(std::ofstream &asset_pack_file) override;
        static std::shared_ptr<model> deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file);

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
        math::bounding_box m_bounding_box;
    };
}