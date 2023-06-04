#pragma once

namespace retro
{
    class utils
    {
    public:
        static std::string extract_file_name(const std::string& path);
        static std::string extract_file_extansion(const std::string& path);
    };
}