#pragma once

#include "asset.h"
#include "asset_pack.h"

namespace retro::assets
{
    class asset_manager
    {
    public:
        asset_manager();

        /* Getters */
        const std::shared_ptr<asset_pack>& get_asset_pack(asset_type type) { return m_asset_packs[type]; }

        /* Functions */
        void initialize();
        void deserialize_packs();
        void serialize_packs();

    private:
        std::map<asset_type, std::shared_ptr<asset_pack>> m_asset_packs;
    };
}
