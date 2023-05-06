#pragma once

#include "renderer/models/model.h"

namespace retro::renderer
{
    class model_loader
    {
    public:
        static std::shared_ptr<model> load_model_from_file(const std::string &file_path);

    private:
    };
}