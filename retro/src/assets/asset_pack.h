#pragma once

#include "assets/asset.h"

namespace retro::assets
{
    class asset_pack
    {
    public:
        asset_pack(asset_type type);

        void save_asset(const std::shared_ptr<asset> &asset);

        void serialize_pack(const std::string &file_path);
        void deserialize_pack(const std::string& file_path);
        
        static void write_string(const std::string &string, std::ofstream &asset_pack);
        static std::string read_string(std::ifstream& asset_pack);

    private:
        asset_type m_type;
        std::unordered_map<uint64_t, std::shared_ptr<asset>> m_assets;
    };
}