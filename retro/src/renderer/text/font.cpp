#include "rtpch.h"

#include "font.h"

#include "text.h"
#include "renderer/textures/texture.h"

namespace retro::renderer
{
    font::font(const std::string& file_path, int glyph_size)
    {
        m_glyph_size = glyph_size;
        FT_Library font_library;
        if (FT_Init_FreeType(&font_library))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font library!");
        }

        if (FT_New_Face(font_library, file_path.c_str(), 0, &m_font_face))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font file!");
        }

        FT_Set_Pixel_Sizes(m_font_face, 0, glyph_size);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        setup_buffers();
        construct_atlas();

        FT_Done_Face(m_font_face);
        FT_Done_FreeType(font_library);
    }

    font::~font()
    {
    }

    void font::setup_buffers()
    {
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
        const int atlas_width = MAX_FONT_GLYPHS * m_glyph_size;
        const int atlas_height = m_glyph_size;

        // Create the empty atlas image
        const auto atlas_data = new unsigned char[atlas_width * atlas_height]; // RED format
        memset(atlas_data, 0, atlas_width * atlas_height); // Set initial pixels to transparent

        int atlas_x = 0; // Current X position within the atlas

        // Iterate over the glyphs
        for (unsigned char c = 0; c < MAX_FONT_GLYPHS; c++)
        {
            FT_Load_Char(m_font_face, c, FT_LOAD_RENDER);

            // Retrieve the glyph bitmap
            const FT_GlyphSlot glyph = m_font_face->glyph;
            const FT_Bitmap bitmap = glyph->bitmap;

            // Calculate the glyph position and size within the atlas
            int glyph_x = atlas_x;
            int glyph_y = 0;
            int glyph_width = bitmap.width;
            int glyph_height = bitmap.rows;

            // Render the glyph onto the atlas image
            for (int y = 0; y < glyph_height; y++)
            {
                for (int x = 0; x < glyph_width; x++)
                {
                    int atlas_index = (glyph_y + y) * atlas_width + (glyph_x + x);
                    int bitmap_index = y * bitmap.width + x;

                    // Copy the glyph bitmap data to the atlas
                    atlas_data[atlas_index] = bitmap.buffer[bitmap_index]; // Alpha channel
                }
            }

            // Calculate the texture coordinates
            float u1 = static_cast<float>(glyph_x) / atlas_width;
            float v1 = 0.0f;
            float u2 = static_cast<float>(glyph_x + glyph_width) / atlas_width;
            float v2 = static_cast<float>(glyph_height) / atlas_height;

            // Store glyph information
            glyph_data glyph_data = {
                glm::ivec2(m_font_face->glyph->bitmap.width, m_font_face->glyph->bitmap.rows),
                glm::ivec2(m_font_face->glyph->bitmap_left, m_font_face->glyph->bitmap_top),
                u1, v1, u2, v2,
                static_cast<unsigned int>(m_font_face->glyph->advance.x)
            };

            m_glyphs_data.insert(std::make_pair(c, glyph_data));

            // Update the atlas X position for the next glyph
            atlas_x += m_glyph_size;
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
