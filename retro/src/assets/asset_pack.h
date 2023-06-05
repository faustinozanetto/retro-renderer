#pragma once

#include "asset.h"

namespace retro::assets
{
    class asset_pack
    {
    public:
        asset_pack(asset_type type, const std::string& file_path);

        /* Getters */
        asset_type get_type() const { return m_type; }
        std::unordered_map<uint64_t, std::shared_ptr<asset>>& get_assets() { return m_assets; }

        template<class T, asset_type type>
        const std::shared_ptr<T>& get_asset(const std::string& asset_file_name)
        {
            std::shared_ptr<T> found_asset;
            for (auto& asset : m_assets)
            {
                if (asset.second->get_metadata().type == type)
                {
                    std::shared_ptr<T> casted_asset = std::dynamic_pointer_cast<T>(
                        asset.second);
                    if (casted_asset && casted_asset->get_metadata().file_name == asset_file_name)
                    {
                        found_asset = casted_asset;
                        break;
                    }
                }
            }
            assert(found_asset, "Could not find asset!");
            return found_asset;
        }

        /* Functions */
        void save_asset(const std::shared_ptr<asset>& asset);

        void serialize_pack();
        void deserialize_pack();

        /* Utilities */
        static void write_string(const std::string& string, std::ofstream& asset_pack);
        static std::string read_string(std::ifstream& asset_pack);

    private:
        void serialize_asset_metadata(const asset_metadata& asset_metadata, std::ofstream& asset_pack_file);
        asset_metadata deserialize_asset_metadata(std::ifstream& asset_pack_file);
        
        asset_type m_type;
        std::string m_file_path;
        std::unordered_map<uint64_t, std::shared_ptr<asset>> m_assets;
    };
}
