#pragma once

#include <string>

namespace retro
{
    class utils
    {
    public:
        static std::string extract_file_name(const std::string& path);
        static std::string extract_file_extension(const std::string& path);
    };
}