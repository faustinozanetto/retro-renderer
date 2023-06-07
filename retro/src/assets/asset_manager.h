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
        std::unordered_map<std::string, std::shared_ptr<asset_pack>> &get_asset_packs() { return m_asset_packs; }
        std::unordered_map<asset_type, std::vector<std::shared_ptr<asset>>> &get_assets() { return m_assets; }
        const std::shared_ptr<asset_pack> &get_asset_pack(const std::string &asset_pack_name) const;
        const std::unordered_set<std::shared_ptr<asset_pack>> &get_deserialized_asset_packs() const { return m_deserialized_asset_packs; }

        template <class T, asset_type P>
        std::shared_ptr<T> get_asset(const std::string &asset_file_name)
        {
            std::shared_ptr<T> found_asset;
            for (auto &asset_category : m_assets)
            {
                if (asset_category.first == P)
                {
                    for (auto &asset : asset_category.second)
                    {
                        found_asset = std::dynamic_pointer_cast<T>(asset);
                        if (found_asset && asset_file_name.compare(found_asset->get_metadata().file_name) == 0)
                        {
                            return found_asset;
                        }
                    }
                }
            }
            return nullptr;
        }

        /* Functions */
        void register_asset_pack(const std::shared_ptr<asset_pack> &asset_pack);
        void register_asset(const std::shared_ptr<asset> &asset_object);

        void deserialize_packs();
        void serialize_packs() const;

    private:
        std::unordered_set<std::shared_ptr<asset_pack>> m_deserialized_asset_packs;
        std::unordered_map<asset_type, std::vector<std::shared_ptr<asset>>> m_assets;
        std::unordered_map<std::string, std::shared_ptr<asset_pack>> m_asset_packs;
    };
}
