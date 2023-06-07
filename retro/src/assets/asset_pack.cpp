#include "rtpch.h"
#include "asset_pack.h"

#include "audio/sound.h"
#include "renderer/models/model.h"
#include "renderer/shaders/shader.h"
#include "renderer/text/font.h"
#include "renderer/materials/material.h"
#include "renderer/textures/texture.h"

namespace retro::assets
{
    asset_pack::asset_pack(const std::string& file_path)
    {
        m_file_path = file_path;
    }

    void asset_pack::save_asset(const std::shared_ptr<asset>& asset)
    {
        m_assets.insert(std::make_pair(asset->get_metadata().uuid, asset));
    }

    void asset_pack::serialize_pack()
    {
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started serializing asset pack.");
        RT_TRACE("  - File Path: '{}'", m_file_path);
        std::ofstream asset_pack_file(m_file_path, std::ios::out | std::ios::binary | std::ios::trunc);

        // Write the number of assets in the pack
        const size_t num_assets = m_assets.size();
        asset_pack_file.write(reinterpret_cast<const char*>(&num_assets), sizeof(num_assets));

        for (const auto& asset : m_assets)
        {
            // Serialize the asset's metadata
            RT_ASSERT_MSG(asset.second, "Asset has invalid asset pointer!");
            
            const asset_metadata& metadata = asset.second->get_metadata();
            serialize_asset_metadata(metadata, asset_pack_file);
            // Perform custom asset serialization
            asset.second->serialize(asset_pack_file);
        }

        asset_pack_file.close();
        RT_TRACE("Retro Renderer | Asset pack serialized successfully!");
        RT_SEPARATOR();
    }

    void asset_pack::deserialize_pack()
    {
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started deserializing asset pack.");
        RT_TRACE("  - File Path: '{}'", m_file_path);
        m_assets.clear();

        std::ifstream asset_pack_file(m_file_path, std::ios::binary);

        // Deserialize the number of assets in the pack
        size_t num_assets;
        asset_pack_file.read(reinterpret_cast<char*>(&num_assets), sizeof(num_assets));

        for (size_t i = 0; i < num_assets; ++i)
        {
            // Check if we have reached the end of the file
            if (!asset_pack_file) break;

            // Deserialize the asset's metadata
            asset_metadata metadata = deserialize_asset_metadata(asset_pack_file);

            RT_TRACE("Retro Renderer | Asset Deserialized: {} {} {} {}", metadata.uuid, metadata.file_path,
                     metadata.file_name, static_cast<int>(metadata.type));

            std::shared_ptr<asset> asset;

            // Create the appropriate asset instance based on the type
            switch (metadata.type)
            {
            case asset_type::shader:
                {
                    asset = renderer::shader::deserialize(metadata, asset_pack_file);
                    break;
                }
            case asset_type::model:
                {
                    asset = renderer::model::deserialize(metadata, asset_pack_file);
                    break;
                }

            case asset_type::texture:
                {
                    asset = renderer::texture::deserialize(metadata, asset_pack_file);
                    break;
                }
            case asset_type::sound:
                {
                    asset = audio::sound::deserialize(metadata, asset_pack_file);
                    break;
                }
            case asset_type::font:
                {
                    asset = renderer::font::deserialize(metadata, asset_pack_file);
                    break;
                }
            case asset_type::material:
                {
                    asset = renderer::material::deserialize(metadata, asset_pack_file);
                    break;
                }
            }

            RT_ASSERT_MSG(asset, "Failed to deserialized asset: '{}'!", name);

            // Store the asset in the map using its UUID as the key
            m_assets[asset->get_metadata().uuid] = std::move(asset);
        }
        RT_TRACE("Retro Renderer | Asset pack successfully deserialized!");
        RT_SEPARATOR();
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

    void asset_pack::serialize_asset_metadata(const asset_metadata& asset_metadata, std::ofstream& asset_pack_file)
    {
        RT_ASSERT_MSG(asset_metadata.file_name.c_str(), "Invalid asset metadata!");
        // Serialize the asset's type
        asset_pack_file.write(reinterpret_cast<const char*>(&asset_metadata.type), sizeof(asset_metadata.type));

        // Serialize the asset's file name
        const size_t file_name_length = asset_metadata.file_name.size();
        asset_pack_file.write(reinterpret_cast<const char*>(&file_name_length), sizeof(file_name_length));
        asset_pack_file.write(asset_metadata.file_name.c_str(), file_name_length);

        // Serialize the asset's file path
        const size_t file_path_length = asset_metadata.file_path.size();
        asset_pack_file.write(reinterpret_cast<const char*>(&file_path_length), sizeof(file_path_length));
        asset_pack_file.write(asset_metadata.file_path.c_str(), file_path_length);

        // Serialize the asset's UUID
        asset_pack_file.write(reinterpret_cast<const char*>(&asset_metadata.uuid), sizeof(asset_metadata.uuid));
    }

    asset_metadata asset_pack::deserialize_asset_metadata(std::ifstream& asset_pack_file)
    {
        asset_metadata metadata;

        // Deserialize the asset's type
        asset_pack_file.read(reinterpret_cast<char*>(&metadata.type), sizeof(metadata.type));

        // Deserialize the asset's file name
        size_t file_name_length;
        asset_pack_file.read(reinterpret_cast<char*>(&file_name_length), sizeof(file_name_length));
        metadata.file_name.resize(file_name_length);
        asset_pack_file.read(&metadata.file_name[0], file_name_length);

        // Deserialize the asset's file path
        size_t file_path_length;
        asset_pack_file.read(reinterpret_cast<char*>(&file_path_length), sizeof(file_path_length));
        metadata.file_path.resize(file_path_length);
        asset_pack_file.read(&metadata.file_path[0], file_path_length);

        // Deserialize the asset's UUID
        asset_pack_file.read(reinterpret_cast<char*>(&metadata.uuid), sizeof(metadata.uuid));

        return metadata;
    }
}
