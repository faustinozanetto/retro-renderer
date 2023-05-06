#pragma once

#include "renderer/models/mesh.h"

namespace retro::renderer
{
    class model
    {
    public:
        model(const std::vector<std::shared_ptr<mesh>> &meshes);

        const std::vector<std::shared_ptr<mesh>> &get_meshes() const { return m_meshes; }

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
    };
}