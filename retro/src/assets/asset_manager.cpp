#include "rtpch.h"
#include "asset_manager.h"

namespace retro::assets
{
    asset_manager::asset_manager()
    {
        initialize();
    }

    void asset_manager::initialize()
    {
        for (asset_type type : asset_types)
        {
            const std::shared_ptr<asset_pack> pack = std::make_shared<asset_pack>(type);
            m_asset_packs.insert(std::make_pair(type, pack));
        }
    }

    void asset_manager::deserialize_packs()
    {
        for (asset_type type : asset_types)
        {
            m_asset_packs[type]->deserialize_pack(
                std::format("resources/packs/{}.pack", asset::get_asset_type_to_string(type)));
        }
    }

    void asset_manager::serialize_packs()
    {
        for (asset_type type : asset_types)
        {
            m_asset_packs[type]->serialize_pack(std::format("resources/packs/{}s.pack",
                                                            asset::get_asset_type_to_string(type)));
        }
    }
}
