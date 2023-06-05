#pragma once

#include "renderer/text/font.h"

namespace retro::renderer
{
    class font_loader
    {
    public:
        static std::shared_ptr<font> load_font_from_file(const std::string &file_path);
        static std::shared_ptr<font> load_font_from_memory(const char *data, int size);
    };
}