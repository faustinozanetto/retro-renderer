#pragma once

#include "shader.h"
#include "utils/file_watcher.h"
#include "utils/singleton.h"

namespace retro::renderer
{
    class shader_file_watcher : public singleton<shader_file_watcher>
    {
    public:
        shader_file_watcher();
        
        void create_shader_file_watcher(const std::shared_ptr<shader>& shader);

        void on_shader_asset_file_update(const std::string& path, file_watcher_event change_type);
    private:
        std::unordered_map<std::string, std::shared_ptr<FileWatch<std::string>>> m_shader_file_watchers;
    };
}
