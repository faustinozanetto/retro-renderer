#include "rtpch.h"
#include "utils.h"

namespace retro
{
    std::string utils::extract_file_name(const std::string& path)
    {
        const size_t slash_index = path.find_last_of("/\\");
        if (slash_index != std::string::npos)
        {
            return path.substr(slash_index + 1);
        }
        return path;
    }

    std::string utils::extract_file_extension(const std::string& path)
    {
        const size_t dot_index = path.find_last_of('.');
        if (dot_index != std::string::npos)
            return path.substr(dot_index);

        return "";
    }

    std::string utils::trim(const std::string& string)
    {
        // Find the first non-whitespace character
        size_t first = string.find_first_not_of(" \t");

        // If the string is all whitespace, return an empty string
        if (first == std::string::npos)
            return "";

        // Find the last non-whitespace character
        size_t last = string.find_last_not_of(" \t");

        // Extract the trimmed substring
        return string.substr(first, last - first + 1);
    }

    std::string utils::remove_carriage(const std::string& string)
    {
        std::string output = string;
        size_t pos = output.find('\r');
        while (pos != std::string::npos)
        {
            output.erase(pos, 1);
            pos = output.find('\r', pos);
        }
        return output;
    }
}
