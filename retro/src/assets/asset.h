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
    };

    struct asset_metadata
    {
        asset_type type;
        std::string name;
        std::string file_name;
        uuid uuid;

        asset_metadata() = default;

        asset_metadata(asset_type type, const std::string &file_name) : type(type),
                                                                        name(utils::extract_file_name(file_name)),
                                                                        file_name(file_name)

        {
        }
    };

    class asset
    {
    public:
        asset(asset_metadata metadata);

        /* Getters */
        const asset_metadata &get_metadata() const { return m_metadata; }

        /* Functions */
        virtual void serialize(std::ofstream &asset_pack_file) = 0;
        void set_metadata(const asset_metadata &metadata) { m_metadata = metadata; }

        /* Utilities */
        static std::string get_asset_type_to_string(asset_type asset_type);

    protected:
        asset_metadata m_metadata;
    };
}
