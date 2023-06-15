#include "rtpch.h"
#include "font_loader.h"

#include <filesystem>

#include FT_FREETYPE_H

namespace retro::renderer
{
    std::shared_ptr<font> font_loader::load_font_from_file(const std::string& file_path)
    {
        RT_PROFILE;
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading font from file '{0}'", file_path);
        FT_Library font_library;
        if (FT_Init_FreeType(&font_library))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font library!");
        }

        font_data font_data;
        font_data.glyph_size = 48;
        font_data.font_face = nullptr;

        if (FT_New_Face(font_library, file_path.c_str(), 0, &font_data.font_face))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font file!");
        }

        FT_Set_Pixel_Sizes(font_data.font_face, 0, font_data.glyph_size);
        const std::shared_ptr<font>& created_font = std::make_shared<font>(file_path, font_data);
        FT_Done_Face(font_data.font_face);
        FT_Done_FreeType(font_library);
        RT_SEPARATOR();

        return created_font;
    }

    std::shared_ptr<font> font_loader::load_font_from_memory(const char* data, int size)
    {
        RT_PROFILE;
        RT_SEPARATOR();
        RT_TRACE("Retro Renderer | Started loading font from memory");
        FT_Library font_library;
        if (FT_Init_FreeType(&font_library))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font library!");
        }

        font_data font_data;
        font_data.glyph_size = 48;
        font_data.font_face = nullptr;

        if (FT_New_Memory_Face(font_library, reinterpret_cast<const unsigned char*>(data), size, 0,
                               &font_data.font_face))
        {
            RT_ASSERT_MSG(false, "Retro Renderer | Could not initalize font file!");
        }

        FT_Set_Pixel_Sizes(font_data.font_face, 0, font_data.glyph_size);
        const std::shared_ptr<font>& created_font = std::make_shared<font>("from_memory", font_data);
        FT_Done_Face(font_data.font_face);
        FT_Done_FreeType(font_library);
        RT_TRACE("Retro Renderer | Font loaded from memory successfully!");
        RT_SEPARATOR();

        return created_font;
    }
}
