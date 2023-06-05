#pragma once

#include "asset.h"
#include "asset_pack.h"
#include "utils/singleton.h"

namespace retro::assets
{
    class asset_manager : public singleton<asset_manager>
    {
    public:
        asset_manager();
        
        /* Getters */
        std::unordered_map<asset_type, std::shared_ptr<asset_pack>>& get_asset_packs() { return m_asset_packs; }
        std::unordered_map<asset_type, std::vector<asset*>>& get_assets() { return m_assets; }
        std::shared_ptr<asset_pack>& get_asset_pack(asset_type type);

        /* Functions */
        void register_asset(asset* asset);
        void deserialize_packs();
        void serialize_packs();

    private:
        std::unordered_map<asset_type, std::vector<asset*>> m_assets;
        std::unordered_map<asset_type, std::shared_ptr<asset_pack>> m_asset_packs;
    };
}
