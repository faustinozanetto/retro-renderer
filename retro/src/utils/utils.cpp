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
}
