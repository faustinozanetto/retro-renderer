#pragma once

#include "assets/asset.h"

namespace retro::assets
{
    class asset_pack
    {
    public:
        asset_pack(asset_type type);

        /* Getters */
        asset_type get_type() const { return m_type; }
        const std::unordered_map<uint64_t, std::shared_ptr<asset>>& get_assets() const { return m_assets; }

        template<typename T, asset_type type>
        std::shared_ptr<T> get_asset(const std::string& asset_name)
        {
            std::shared_ptr<T> found_asset;
            for (const auto& asset : m_assets)
            {
                if (asset.second->get_type() == type)
                {
                    std::shared_ptr<T> casted_asset = std::dynamic_pointer_cast<T>(
                        asset.second);
                    if (casted_asset && casted_asset->get_name() == asset_name)
                    {
                        found_asset = casted_asset;
                        break;
                    }
                }
            }
            return found_asset;
        }

        /* Functions */
        void save_asset(const std::shared_ptr<asset>& asset);

        void serialize_pack(const std::string& file_path) const;
        void deserialize_pack(const std::string& file_path);

        /* Utilities */
        static void write_string(const std::string& string, std::ofstream& asset_pack);
        static std::string read_string(std::ifstream& asset_pack);

    private:
        asset_type m_type;
        std::unordered_map<uint64_t, std::shared_ptr<asset>> m_assets;
    };
}
