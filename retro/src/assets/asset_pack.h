#pragma once

#include "asset.h"

#include <unordered_set>

namespace retro::assets
{
    class asset_pack
    {
    public:
        asset_pack(const std::string &name, const std::string &file_path);
        asset_pack(const std::string &name, const std::string &file_path, const std::unordered_set<std::shared_ptr<asset_pack>> &dependencies);

        /* Getters */
        std::unordered_map<uint64_t, std::shared_ptr<asset>> &get_assets() { return m_assets; }

        template <class T>
        std::shared_ptr<T> get_asset(const std::string &asset_file_name)
        {
            std::shared_ptr<T> found_asset;
            for (auto &asset : m_assets)
            {
                found_asset = std::dynamic_pointer_cast<T>(
                    asset.second);
                if (found_asset && asset_file_name.compare(found_asset->get_metadata().file_name) == 0)
                {
                    return found_asset;
                }
            }
            assert(false, "Could not find asset!");
            return nullptr;
        }

        /* Functions */
        void save_asset(const std::shared_ptr<asset> &asset);

        void serialize_pack();
        void deserialize_pack();

        /* Getters */
        const std::string &get_name() const { return m_name; }
        const std::string &get_file_path() const { return m_file_path; }
        const std::unordered_set<std::shared_ptr<asset_pack>> &get_dependencies() const { return m_dependencies; }

        /* Operators */
        bool operator<(const asset_pack &other)
        {
            return get_name() < other.get_name();
        }

        /* Utilities */
        static void write_string(const std::string &string, std::ofstream &asset_pack);
        static std::string read_string(std::ifstream &asset_pack);

    private:
        void serialize_asset_metadata(const asset_metadata &asset_metadata, std::ofstream &asset_pack_file);
        asset_metadata deserialize_asset_metadata(std::ifstream &asset_pack_file);

        std::string m_name;
        std::string m_file_path;
        std::unordered_set<std::shared_ptr<asset_pack>> m_dependencies;
        std::unordered_map<uint64_t, std::shared_ptr<asset>> m_assets;
    };
}
