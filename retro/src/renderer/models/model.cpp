#include "rtpch.h"
#include "model.h"

namespace retro::renderer
{
    model::model(const std::vector<std::shared_ptr<mesh>> &meshes)
    {
        m_meshes = meshes;
    }
}