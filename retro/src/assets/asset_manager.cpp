#include "rtpch.h"
#include "asset_manager.h"

namespace retro::assets
{
    asset_manager::asset_manager()
    {
    }

    const std::shared_ptr<asset_pack> &asset_manager::get_asset_pack(const std::string &asset_pack_name) const
    {
        RT_ASSERT_MSG(m_asset_packs.contains(asset_pack_name), "Unkown asset pack!");
        return m_asset_packs.at(asset_pack_name);
    }

    void asset_manager::register_asset_pack(const std::shared_ptr<asset_pack> &asset_pack)
    {
        m_asset_packs.insert(std::make_pair(asset_pack->get_name(), asset_pack));
    }

    void asset_manager::register_asset(const std::shared_ptr<asset> &asset_object)
    {
        m_assets[asset_object->get_metadata().type].push_back(asset_object);
    }

    void asset_manager::deserialize_packs()
    {
        for (const auto &[asset_pack_name, asset_pack] : m_asset_packs)
        {
            // Check if the asset pack has already been deserialized
            if (m_deserialized_asset_packs.find(asset_pack) != m_deserialized_asset_packs.end())
            {
                RT_TRACE("Retro Renderer | Asset pack already deserialized!");
                RT_SEPARATOR();
                return;
            }

            // Check if the asset pack has dependencies
            for (const auto &dependency : asset_pack->get_dependencies())
            {
                dependency->deserialize_pack();
                RT_TRACE("Retro Renderer | Deserialized dependency asset pack '{}' for asset pack '{}'!", dependency->get_name(), asset_pack_name);
                // Mark the asset type as deserialized
                m_deserialized_asset_packs.insert(dependency);
            }

            // Deserialize the asset pack
            asset_pack->deserialize_pack();

            // Mark the asset type as deserialized
            m_deserialized_asset_packs.insert(asset_pack);
        }
    }

    void asset_manager::serialize_packs() const
    {
        for (const auto &asset_pack : m_asset_packs)
        {
            asset_pack.second->serialize_pack();
        }
    }
}
