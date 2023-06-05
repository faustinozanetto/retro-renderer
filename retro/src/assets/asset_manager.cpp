#include "rtpch.h"
#include "asset_manager.h"

namespace retro::assets
{
    asset_manager::asset_manager()
    {
        for (asset_type type : asset_types)
        {
            const std::string& file_path =
                std::format("resources/packs/{}s.pack", asset::get_asset_type_to_string(type));
            const std::shared_ptr<asset_pack> pack = std::make_shared<asset_pack>(type, file_path);
            m_asset_packs.insert(std::make_pair(type, pack));
        }
    }

    std::shared_ptr<asset_pack>& asset_manager::get_asset_pack(asset_type type)
    {
        RT_ASSERT_MSG(m_asset_packs[type], "Asset pack not found!");
        return m_asset_packs[type];
    }

    void asset_manager::register_asset(asset* asset)
    {
        m_assets[asset->get_metadata().type].push_back(asset);
    }

    void asset_manager::deserialize_packs()
    {
        for (asset_type type : asset_types)
        {
            RT_ASSERT_MSG(m_asset_packs[type], "Asset pack not found!");
            m_asset_packs[type]->deserialize_pack();
        }
    }

    void asset_manager::serialize_packs()
    {
        for (asset_type type : asset_types)
        {
            RT_ASSERT_MSG(m_asset_packs[type], "Asset pack not found!");
            m_asset_packs[type]->serialize_pack();
        }
    }
}
