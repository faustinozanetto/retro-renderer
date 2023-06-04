#include "rtpch.h"
#include "sound.h"

#include <AL/al.h>

namespace retro::audio
{
    sound::sound(const std::string& file_name, const sound_data& sound_data) : asset({assets::asset_type::sound, file_name})
    {
        m_data = sound_data;
        RT_TRACE("Retro Renderer | Started loading sound from file.");
        RT_TRACE("    - Name: {0}", file_name.c_str());
        RT_TRACE("    - Frequency: {0}", m_data.frequency);
        RT_TRACE("    - Lenght: {0}", m_data.length);
        RT_TRACE("    - Bit Rate: {0}", m_data.bit_rate);
        RT_TRACE("    - Size: {0}", m_data.size);
        RT_TRACE("    - Channels: {0}", m_data.channels);

        // Generate openal buffers
        alGenBuffers(1, &m_sound_buffer);
        alBufferData(m_sound_buffer, AL_FORMAT_STEREO16, m_data.data,
                     m_data.size,
                     static_cast<ALsizei>(m_data.frequency));
        RT_TRACE("Retro Renderer | Sound loaded successfully!");
    }

    sound::~sound()
    {
        alDeleteBuffers(1, &m_sound_buffer);
    }

    void sound::serialize(std::ofstream& asset_pack_file)
    {
        // Serialize the sounds's data size
        const size_t data_size = m_data.size;
        asset_pack_file.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));

        // Serialize the sound's metadata (frequency, length, bit_rate, channels, size)
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.frequency), sizeof(m_data.frequency));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.length), sizeof(m_data.length));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.bit_rate), sizeof(m_data.bit_rate));
        asset_pack_file.write(reinterpret_cast<const char*>(&m_data.channels), sizeof(m_data.channels));

        // Serialize the texture's data
        asset_pack_file.write(static_cast<const char*>(m_data.data), data_size);
    }

    std::shared_ptr<sound> sound::deserialize(const assets::asset_metadata &metadata, std::ifstream& asset_pack_file)
    {
        sound_data data;
        // Deserialize the sound data size
        size_t data_size;
        asset_pack_file.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

        // Allocate memory for the sound data
        data.data = new char[data_size];
        
        // Deserialize the sound's metadata (frequency, length, bit_rate, channels)
        asset_pack_file.read(reinterpret_cast<char*>(&data.frequency), sizeof(data.frequency));
        asset_pack_file.read(reinterpret_cast<char*>(&data.length), sizeof(data.length));
        asset_pack_file.read(reinterpret_cast<char*>(&data.bit_rate), sizeof(data.bit_rate));
        asset_pack_file.read(reinterpret_cast<char*>(&data.channels), sizeof(data.channels));
        
        asset_pack_file.read(reinterpret_cast<char*>(data.data), data_size);
        
        data.size = data_size;

        return std::make_shared<sound>(metadata.file_name, data);
    }
}
