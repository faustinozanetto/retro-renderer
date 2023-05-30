#pragma once

#include "renderer/buffers/vertex_array_object.h"
#include "renderer/buffers/vertex_buffer_object.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

#define MAX_FONT_GLYPHS 128

namespace retro::renderer
{
    struct glyph_data
    {
        glm::ivec2 size; // Size of glyph
        glm::ivec2 bearing; // Offset from baseline to left/top of glyph
        float u1, v1, u2, v2;
        unsigned int advance; // Horizontal offset to advance to next glyph
    };

    class font
    {
    public:
        font(const std::string& file_path, int glyph_size = 48);
        ~font();

        const std::shared_ptr<vertex_array_object>& get_font_vao() const { return m_font_vao; }
        const std::map<char, glyph_data>& get_glyphs_data() const { return m_glyphs_data; }
        uint32_t get_glyph_atlas() const { return m_glyph_atlas; }

    private:
        void setup_buffers();
        void construct_atlas();

        FT_Face m_font_face;
        int m_glyph_size;
        std::map<char, glyph_data> m_glyphs_data;
        uint32_t m_glyph_atlas;

        std::shared_ptr<vertex_array_object> m_font_vao;
    };
}
