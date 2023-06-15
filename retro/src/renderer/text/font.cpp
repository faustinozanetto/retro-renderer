#include "rtpch.h"
#include "font.h"

#include "renderer/text/text.h"
#include "renderer/text/font_loader.h"
#include "renderer/textures/texture.h"

namespace retro::renderer
{
    font::font(const std::string& file_name, const font_data& font_data) : asset(
        {assets::asset_type::font, file_name})
    {
        m_data = font_data;

        RT_TRACE("Font Information:");
        RT_TRACE("  - Family Name: '{}'", m_data.font_face->family_name);
        RT_TRACE("  - Style Name: '{}'", m_data.font_face->style_name);
        RT_TRACE("  - Number of Glyphs: '{}'", m_data.font_face->num_glyphs);
        RT_TRACE("  - Units Per EM: '{}'", m_data.font_face->units_per_EM);
        RT_TRACE("  - Ascender: '{}'", m_data.font_face->ascender);
        RT_TRACE("  - Descender: '{}'", m_data.font_face->descender);
        RT_TRACE("  - Height: '{}'", m_data.font_face->height);
        RT_TRACE("  - Max Advance Width: '{}'", m_data.font_face->max_advance_width);
        RT_TRACE("  - Max Advance Height: '{}'", m_data.font_face->max_advance_height);
        RT_TRACE("  - Glyphs: '{}'", m_data.font_face->num_glyphs);

        setup_buffers();
        construct_atlas();
    }

    font::~font()
    {
    }

    void font::serialize(std::ofstream& asset_pack_file)
    {
        RT_PROFILE;
        // Read font file.
        std::ifstream font_file(m_metadata.file_path, std::ios::binary | std::ios::ate);
        RT_ASSERT_MSG(font_file.is_open(), "Failed to open sound file while serializing asset!")

        const std::streamsize size = font_file.tellg();
        font_file.seekg(0, std::ios::beg);

        // Read the file data into a temporary buffer
        std::vector<char> buffer(size);
        if (!font_file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read file: " << m_metadata.file_name << std::endl;
            return;
        }

        // Write the font file size to the asset pack file
        asset_pack_file.write(reinterpret_cast<const char*>(&size), sizeof(std::streamsize));

        // Write the font file data to the asset pack file
        asset_pack_file.write(buffer.data(), size);
    }

    std::shared_ptr<font> font::deserialize(const assets::asset_metadata& metadata, std::ifstream& asset_pack_file)
    {
        RT_PROFILE;
        // Read the font file size from the asset pack file
        std::streamsize data_size;
        asset_pack_file.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

        // Allocate memory for the font data
        std::vector<char> data(data_size);

        // Deserialize the font's data
        asset_pack_file.read(data.data(), data_size);

        auto font = font_loader::load_font_from_memory(data.data(), static_cast<int>(data_size));
        font->set_metadata(metadata);
        return font;
    }

    void font::setup_buffers()
    {
        RT_PROFILE;
        m_font_vao = std::make_shared<vertex_array_object>();
        m_font_vao->bind();
        const std::shared_ptr<vertex_buffer_object> vbo = std::make_shared<
            vertex_buffer_object>(vertex_buffer_object_target::arrays);
        vbo->set_data(vertex_buffer_object_usage::dynamic_draw, sizeof(text_vertex), nullptr);

        std::initializer_list<vertex_buffer_layout_entry>
            layout_elements = {
                {"a_pos_tex_coord", vertex_buffer_entry_type::vec_float4, false}
            };

        const std::shared_ptr<vertex_buffer_layout_descriptor>
            vbo_layout_descriptor = std::make_shared<vertex_buffer_layout_descriptor>(layout_elements);
        vbo->set_layout_descriptor(vbo_layout_descriptor);

        m_font_vao->add_vertex_buffer(vbo);
    }

    void font::construct_atlas()
    {
        RT_PROFILE;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        const int atlas_width = MAX_FONT_GLYPHS * m_data.glyph_size;
        const int atlas_height = m_data.glyph_size;

        // Create the empty atlas image
        const auto atlas_data = new unsigned char[static_cast<size_t>(atlas_width) * atlas_height]; // RED format
        memset(atlas_data, 0, static_cast<size_t>(atlas_width) * atlas_height); // Set initial pixels to transparent

        int atlas_x = 0; // Current X position within the atlas

        // Iterate over the glyphs
        for (unsigned char c = 0; c < MAX_FONT_GLYPHS; c++)
        {
            FT_Load_Char(m_data.font_face, c, FT_LOAD_RENDER);

            // Retrieve the glyph bitmap
            const FT_GlyphSlot glyph = m_data.font_face->glyph;
            const FT_Bitmap bitmap = glyph->bitmap;

            // Calculate the glyph position and size within the atlas
            const int glyph_x = atlas_x;
            const int glyph_width = static_cast<int>(bitmap.width);
            const int glyph_height = static_cast<int>(bitmap.rows);

            // Render the glyph onto the atlas image
            for (int y = 0; y < glyph_height; y++)
            {
                for (int x = 0; x < glyph_width; x++)
                {
                    const int glyph_y = 0;
                    const int atlas_index = (glyph_y + y) * atlas_width + (glyph_x + x);
                    const int bitmap_index = y * static_cast<int>(bitmap.width) + x;

                    // Copy the glyph bitmap data to the atlas
                    atlas_data[atlas_index] = bitmap.buffer[bitmap_index]; // Alpha channel
                }
            }

            // Calculate the texture coordinates
            const float u1 = static_cast<float>(glyph_x) / static_cast<float>(atlas_width);
            const float v1 = 0.0f;
            const float u2 = static_cast<float>(glyph_x + glyph_width) / static_cast<float>(atlas_width);
            const float v2 = static_cast<float>(glyph_height) / static_cast<float>(atlas_height);

            // Store glyph information
            glyph_data glyph_data = {
                glm::ivec2(m_data.font_face->glyph->bitmap.width, m_data.font_face->glyph->bitmap.rows),
                glm::ivec2(m_data.font_face->glyph->bitmap_left, m_data.font_face->glyph->bitmap_top),
                u1, v1, u2, v2,
                static_cast<unsigned int>(m_data.font_face->glyph->advance.x)
            };

            m_glyphs_data.insert(std::make_pair(c, glyph_data));

            // Update the atlas X position for the next glyph
            atlas_x += m_data.glyph_size;
        }

        // Generate texture atlas
        glGenTextures(1, &m_glyph_atlas);
        glBindTexture(GL_TEXTURE_2D, m_glyph_atlas);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_data);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Cleanup
        delete[] atlas_data;
    }
}
