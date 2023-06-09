﻿#pragma once

#include "assets/asset.h"

namespace retro::audio
{
    struct sound_data
    {
        float frequency;
        float length;
        uint32_t bit_rate;
        uint32_t channels;
        int size;
        void* data;
    };

    class sound : public assets::asset
    {
    public:
        sound(const std::string& file_name, const sound_data& sound_data);
        ~sound();

        /* Getters */
        float get_frequency() const { return m_data.frequency; }
        float get_length() const { return m_data.length; }
        uint32_t get_bit_rate() const { return m_data.bit_rate; }
        uint32_t get_channels() const { return m_data.channels; }
        uint32_t get_sound_buffer() const { return m_sound_buffer; }

        /* Functions */

        /* Asset */
        void serialize(std::ofstream& asset_pack_file) override;
        static std::shared_ptr<sound> deserialize(const assets::asset_metadata &metadata, std::ifstream& asset_pack_file);

    private:
        sound_data m_data;
        uint32_t m_sound_buffer;
    };
}
