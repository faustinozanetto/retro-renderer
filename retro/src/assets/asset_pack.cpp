#include "rtpch.h"
#include "asset_pack.h"

#include "renderer/shaders/shader.h"
#include "renderer/textures/texture.h"

namespace retro::assets
{
    asset_pack::asset_pack(asset_type type)
    {
        m_type = type;
    }

    void asset_pack::save_asset(const std::shared_ptr<asset>& asset)
    {
        RT_ASSERT_MSG(m_type == asset->get_type(), "Asset type does not match asset pack type!");

        m_assets.insert(std::make_pair(asset->get_uuid(), asset));
    }

    void asset_pack::serialize_pack(const std::string& file_path)
    {
        std::ofstream pack_file(file_path, std::ios::out | std::ios::binary | std::ios::trunc);

        // Write the number of assets in the pack
        size_t num_assets = m_assets.size();
        pack_file.write(reinterpret_cast<const char*>(&num_assets), sizeof(num_assets));

        for (const auto& asset : m_assets)
        {
            // Write the metadata attributes to the pack file
            pack_file.write(reinterpret_cast<const char*>(&asset.second->get_type()), sizeof(asset.second->get_type()));
            pack_file.write(reinterpret_cast<const char*>(&asset.first), sizeof(asset.first));
            write_string(asset.second->get_name(), pack_file);

            asset.second->serialize(pack_file);
        }

        pack_file.close();
    }

    void asset_pack::deserialize_pack(const std::string& file_path)
    {
        m_assets.clear();

        std::ifstream pack_file(file_path, std::ios::binary);

        // Read the number of assets in the pack
        size_t numAssets;
        pack_file.read(reinterpret_cast<char*>(&numAssets), sizeof(numAssets));

        for (size_t i = 0; i < numAssets; ++i)
        {
            // Check if we have reached the end of the file
            if (!pack_file) break;

            std::shared_ptr<asset> asset;

            // Read the metadata attributes from the pack file
            asset_type type;
            pack_file.read(reinterpret_cast<char*>(&type), sizeof(type));
            uuid uuid;
            pack_file.read(reinterpret_cast<char*>(&uuid), sizeof(uuid));
            std::string name = read_string(pack_file);

            RT_TRACE("Deserialized: {} {} {}", (int)type, uuid, name);

            // Create the appropriate asset instance based on the type
            switch (type)
            {
            case asset_type::shader:
                {
                    RT_TRACE("Read shader file!");
                    asset = renderer::shader::deserialize(name, pack_file);
                    break;
                }

            case asset_type::model:
                {
                    RT_TRACE("Read model file!");
                    break;
                }

            case asset_type::texture:
                {
                    RT_TRACE("Read texture file!");
                    asset = renderer::texture::deserialize(name, pack_file);
                    break;
                }
            }

            RT_ASSERT_MSG(asset, "Failed to deserialized asset: '{}'!", name);

            // Store the asset in the map using its UUID as the key
            m_assets[asset->get_uuid()] = std::move(asset);
        }
        RT_TRACE("Asset pack successfully deserialized!");
    }

    void asset_pack::write_string(const std::string& string, std::ofstream& asset_pack)
    {
        size_t size = string.size();
        asset_pack.write(reinterpret_cast<const char*>(&size), sizeof(size));
        asset_pack.write(string.c_str(), size);
    }

    std::string asset_pack::read_string(std::ifstream& asset_pack)
    {
        size_t size;
        asset_pack.read(reinterpret_cast<char*>(&size), sizeof(size));
        std::string str(size, '\0');
        asset_pack.read(&str[0], size);
        return str;
    }
}
