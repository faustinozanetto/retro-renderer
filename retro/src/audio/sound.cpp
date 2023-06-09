﻿#include "rtpch.h"
#include "sound.h"

#include "sound_loader.h"

#include <AL/al.h>

namespace retro::audio
{
    sound::sound(const std::string& file_name, const sound_data& sound_data) : asset({
        assets::asset_type::sound, file_name
    })
    {
        m_data = sound_data;
        RT_TRACE("Sound Information:");
        RT_TRACE("    - File Name: {0}", file_name.c_str());
        RT_TRACE("    - Frequency: {0}", m_data.frequency);
        RT_TRACE("    - Length: {0}", m_data.length);
        RT_TRACE("    - Bit Rate: {0}", m_data.bit_rate);
        RT_TRACE("    - Size: {0}", m_data.size);
        RT_TRACE("    - Channels: {0}", m_data.channels);

        // Generate OpenAL buffers
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
        RT_PROFILE;
        // Reaad sound file
        std::ifstream sound_file(m_metadata.file_path, std::ios::binary | std::ios::ate);
        RT_ASSERT_MSG(sound_file.is_open(), "Failed to open sound file while serializing asset!");

        const std::streamsize size = sound_file.tellg();
        sound_file.seekg(0, std::ios::beg);

        // Read the file data into a temporary buffer
        std::vector<char> buffer(size);
        if (!sound_file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read file: " << m_metadata.file_name << std::endl;
            return;
        }

        // Write the texture file size to the asset pack file
        asset_pack_file.write(reinterpret_cast<const char*>(&size), sizeof(std::streamsize));

        // Write the texture file data to the asset pack file
        asset_pack_file.write(buffer.data(), size);
    }

    std::shared_ptr<sound> sound::deserialize(const assets::asset_metadata& metadata,
                                                     std::ifstream& asset_pack_file)
    {
        RT_PROFILE;
        // Read the sound file size from the asset pack file
        size_t data_size;
        asset_pack_file.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

        // Allocate memory for the sound data
        std::vector<char> data(data_size);

        // Deserialize the sound's data
        asset_pack_file.read(data.data(), data_size);
        
        const std::string& file_extension = utils::extract_file_extension(metadata.file_name);
        if (file_extension == ".ogg")
        {
            const std::shared_ptr<sound>& sound = sound_loader::load_ogg_sound_from_memory(data.data(), data_size);
            RT_ASSERT_MSG(sound, "Could not deserialize sound!");
            sound->set_metadata(metadata);
            return sound;
        }
        if (file_extension == ".wav")
        {
            const std::shared_ptr<sound>& sound = sound_loader::load_wav_sound_from_memory(data.data(), data_size);
            RT_ASSERT_MSG(sound, "Could not deserialize sound!");
            sound->set_metadata(metadata);
            return sound;
        }

        RT_ASSERT_MSG(false, "Could not deserialize sound!")
    }
}
