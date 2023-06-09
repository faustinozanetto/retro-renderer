#pragma once

namespace retro
{
    class files
    {
    public:
        static std::string open_file_dialog(const std::string &title, const std::vector<std::string> &filters);
    };
}