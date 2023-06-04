#include "rtpch.h"
#include "asset.h"

namespace retro::assets
{
    asset::asset(asset_metadata metadata)
    {
        m_metadata = std::move(metadata);
    }

    std::string asset::get_asset_type_to_string(asset_type asset_type)
    {
        switch (asset_type)
        {
        case asset_type::shader: return "shader";
        case asset_type::texture: return "texture";
        case asset_type::model: return "model";
        case asset_type::sound: return "sound";
        }
        RT_ASSERT_MSG(false, "Invalid asset type!");
    }
}
