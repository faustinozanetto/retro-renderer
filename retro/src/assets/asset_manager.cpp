#include "rtpch.h"
#include "asset_manager.h"

namespace retro::assets
{
    asset_manager::asset_manager()
    {
    }

    const std::shared_ptr<asset_pack>& asset_manager::get_asset_pack(const std::string& asset_pack_name) const
    {
        return m_asset_packs.at(asset_pack_name);
    }

    void asset_manager::register_asset_pack(const std::string& asset_pack_name)
    {
        const std::string& file_path =
            std::format("resources/packs/{}.pack", asset_pack_name);
        auto pack = std::make_shared<asset_pack>(file_path);
        m_asset_packs.insert(std::make_pair(asset_pack_name, pack));
    }

    void asset_manager::register_asset(asset* asset)
    {
        m_assets[asset->get_metadata().type].push_back(asset);
    }

    void asset_manager::deserialize_packs() const
    {
        for (const auto& asset_pack : m_asset_packs)
        {
            asset_pack.second->deserialize_pack();
        }
    }

    void asset_manager::serialize_packs() const
    {
        for (const auto& asset_pack : m_asset_packs)
        {
            asset_pack.second->serialize_pack();
        }
    }
}
