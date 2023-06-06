﻿#include "rtpch.h"
#include "shader_file_watcher.h"

#include "assets/asset_manager.h"
#include "core/application.h"

namespace retro::renderer
{
    shader_file_watcher::shader_file_watcher()
    {
    }

    void shader_file_watcher::create_shader_file_watcher(const std::shared_ptr<shader>& shader)
    {
        auto shader_file_watcher = std::make_shared<FileWatch<
            std::string>>(
            shader->get_metadata().file_path.c_str(), BIND_EVENT_FN(on_shader_asset_file_update));
        m_shader_file_watchers.insert(std::make_pair(shader->get_metadata().file_name, shader_file_watcher));
    }

    void shader_file_watcher::on_shader_asset_file_update(const std::string& path, file_watcher_event change_type)
    {
        RT_TRACE("Shader file updated: '{}'", path);
        // If shader file was modified, perform hot reload
        if (change_type == file_watcher_event::modified)
        {
            core::application::get().submit_to_main_thread([path, this]()
            {
                const std::string& file_name = utils::extract_file_name(path);
                shader* shader_asset = nullptr;
                for (assets::asset* asset : assets::asset_manager::get().get_assets()[assets::asset_type::shader])
                {
                    auto* casted_asset = dynamic_cast<shader*>(asset);
                    if (casted_asset && casted_asset->get_metadata().file_name == file_name)
                    {
                        shader_asset = casted_asset;
                        break;
                    }
                }
                RT_ASSERT_MSG(shader_asset, "Could not find shader asset when performing shader hot reload!");
                shader_asset->recompile();
            });
        }
    }
}