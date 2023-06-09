﻿#include "rtpch.h"
#include "asset.h"

#include "asset_manager.h"

namespace retro::assets
{
    asset::asset(asset_metadata metadata)
    {
        m_metadata = std::move(metadata);
        asset_manager::get().register_asset(std::shared_ptr<asset>(this));
    }

    const char* asset::get_asset_type_to_string(asset_type asset_type)
    {
        RT_PROFILE;
        switch (asset_type)
        {
        case asset_type::shader: return "shader";
        case asset_type::texture: return "texture";
        case asset_type::model: return "model";
        case asset_type::sound: return "sound";
        case asset_type::font: return "font";
        case asset_type::material: return "material";
        }
        RT_ASSERT_MSG(false, "Invalid asset type!");
    }
}
