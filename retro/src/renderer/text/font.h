#pragma once

#include "renderer/buffers/vertex_array_object.h"
#include "renderer/buffers/vertex_buffer_object.h"
#include "assets/asset.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

#define MAX_FONT_GLYPHS 128

namespace retro::renderer
{
    struct glyph_data
    {
        glm::ivec2 size;    // Size of glyph
        glm::ivec2 bearing; // Offset from baseline to left/top of glyph
        float u1, v1, u2, v2;
        unsigned int advance; // Horizontal offset to advance to next glyph
    };

    struct font_data
    {
        int glyph_size;
        FT_Face font_face;
    };

    class font : public assets::asset
    {
    public:
        font(const std::string &file_name, const font_data &font_data);
        ~font();

        /* Getters */
        const std::shared_ptr<vertex_array_object> &get_font_vao() const { return m_font_vao; }
        const std::map<char, glyph_data> &get_glyphs_data() const { return m_glyphs_data; }
        uint32_t get_glyph_atlas() const { return m_glyph_atlas; }

        /* Asset */
        void serialize(std::ofstream &asset_pack_file) override;
        static std::shared_ptr<font> deserialize(const assets::asset_metadata &metadata, std::ifstream &asset_pack_file);

    private:
        void setup_buffers();
        void construct_atlas();

        font_data m_data;
        std::map<char, glyph_data> m_glyphs_data;
        uint32_t m_glyph_atlas;

        std::shared_ptr<vertex_array_object> m_font_vao;
    };
}
