#pragma once

#include "utils/utils.h"
#include "utils/uuid.h"

namespace retro::assets
{
    enum class asset_type
    {
        shader = 0,
        texture,
        model,
        sound,
        font,
        material,
    };

    constexpr asset_type asset_types[]{
        asset_type::shader, asset_type::texture, asset_type::model, asset_type::sound, asset_type::font, asset_type::material,
    };

    struct asset_metadata
    {
        asset_type type;
        std::string file_name;
        std::string file_path;
        uuid uuid;

        asset_metadata() = default;

        asset_metadata(asset_type type, const std::string& file_path) : type(type),
                                                                        file_name(utils::extract_file_name(file_path)),
                                                                        file_path(file_path)

        {
        }
    };

    class asset
    {
    public:
        asset(asset_metadata metadata);
        virtual ~asset() = default;

        /* Getters */
        const asset_metadata& get_metadata() const { return m_metadata; }

        /* Functions */
        virtual void serialize(std::ofstream& asset_pack_file) = 0;
        void set_metadata(const asset_metadata& metadata) { m_metadata = metadata; }

        /* Utilities */
        static const char* get_asset_type_to_string(asset_type asset_type);

    protected:
        asset_metadata m_metadata;
    };
}
