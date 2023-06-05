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
        const std::shared_ptr<asset_pack> models_asset_pack = std::make_shared<asset_pack>(asset_type::model);
        m_asset_packs.insert(std::make_pair(asset_type::model, models_asset_pack));

        const std::shared_ptr<asset_pack> textures_asset_pack = std::make_shared<asset_pack>(asset_type::texture);
        m_asset_packs.insert(std::make_pair(asset_type::texture, textures_asset_pack));

        const std::shared_ptr<asset_pack> shaders_asset_pack = std::make_shared<asset_pack>(asset_type::shader);
        m_asset_packs.insert(std::make_pair(asset_type::shader, shaders_asset_pack));

        const std::shared_ptr<asset_pack> sounds_asset_pack = std::make_shared<asset_pack>(asset_type::sound);
        m_asset_packs.insert(std::make_pair(asset_type::sound, sounds_asset_pack));

        const std::shared_ptr<asset_pack> fonts_asset_pack = std::make_shared<asset_pack>(asset_type::font);
        m_asset_packs.insert(std::make_pair(asset_type::font, fonts_asset_pack));
    }

    void asset_manager::deserialize_packs()
    {
        m_asset_packs[asset_type::model]->deserialize_pack(std::format("resources/packs/{}.pack", "models"));
        m_asset_packs[asset_type::texture]->deserialize_pack(std::format("resources/packs/{}.pack", "textures"));
        m_asset_packs[asset_type::shader]->deserialize_pack(std::format("resources/packs/{}.pack", "shaders"));
        m_asset_packs[asset_type::sound]->deserialize_pack(std::format("resources/packs/{}.pack", "sound"));
        m_asset_packs[asset_type::font]->deserialize_pack(std::format("resources/packs/{}.pack", "font"));
    }

    void asset_manager::serialize_packs()
    {
        m_asset_packs[asset_type::model]->serialize_pack(std::format("resources/packs/{}.pack", "models"));
        m_asset_packs[asset_type::texture]->serialize_pack(std::format("resources/packs/{}.pack", "textures"));
        m_asset_packs[asset_type::shader]->serialize_pack(std::format("resources/packs/{}.pack", "shaders"));
        m_asset_packs[asset_type::sound]->serialize_pack(std::format("resources/packs/{}.pack", "sound"));
        m_asset_packs[asset_type::font]->serialize_pack(std::format("resources/packs/{}.pack", "font"));
    }
}
