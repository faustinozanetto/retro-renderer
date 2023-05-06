#include "rtpch.h"
#include "model_loader.h"

namespace retro::renderer
{
    std::shared_ptr<model> model_loader::load_model_from_file(const std::string &file_path)
    {
        RT_TRACE("Retro Renderer | Started loading model from file.");
        RT_TRACE("  - File Path: '{0}'", file_path);

        RT_TRACE("Retro Renderer | Model loaded successfully.");
        return std::shared_ptr<model>();
    }
}