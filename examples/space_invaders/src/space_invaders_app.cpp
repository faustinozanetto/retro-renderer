#include "space_invaders_app.h"

#include "core/entry_point.h"

#include <unordered_set>

space_invaders_app::space_invaders_app() : application("./")
{
    const auto &models_asset_pack = std::make_shared<retro::assets::asset_pack>("models", "resources/packs/models.pack");
    retro::assets::asset_manager::get().register_asset_pack(models_asset_pack);

    const auto &fonts_asset_pack = std::make_shared<retro::assets::asset_pack>("fonts", "resources/packs/fonts.pack");
    retro::assets::asset_manager::get().register_asset_pack(fonts_asset_pack);

    const auto &shaders_asset_pack = std::make_shared<retro::assets::asset_pack>("shaders", "resources/packs/shaders.pack");
    retro::assets::asset_manager::get().register_asset_pack(shaders_asset_pack);

    const auto &textures_asset_pack = std::make_shared<retro::assets::asset_pack>("textures", "resources/packs/textures.pack");
    retro::assets::asset_manager::get().register_asset_pack(textures_asset_pack);

    std::unordered_set<std::shared_ptr<retro::assets::asset_pack>> materials_asset_pack_dependencies;
    materials_asset_pack_dependencies.insert(textures_asset_pack);
    const auto &materials_asset_pack = std::make_shared<retro::assets::asset_pack>("materials", "resources/packs/materials.pack", materials_asset_pack_dependencies);
    retro::assets::asset_manager::get().register_asset_pack(materials_asset_pack);

    const auto &sounds_asset_pack = std::make_shared<retro::assets::asset_pack>("sounds", "resources/packs/sounds.pack");
    retro::assets::asset_manager::get().register_asset_pack(sounds_asset_pack);

    initialize_managers();
}

space_invaders_app::~space_invaders_app()
{
}

void space_invaders_app::on_update()
{
    m_game_manager->update_game();
}

void space_invaders_app::initialize_managers()
{
    m_game_manager = std::make_shared<game_manager>();
}

void space_invaders_app::on_handle_event(retro::events::base_event &event)
{
    retro::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<retro::events::key_pressed_event>(BIND_EVENT_FN(space_invaders_app::on_key_pressed));
    dispatcher.dispatch<retro::events::window_resize_event>(BIND_EVENT_FN(space_invaders_app::on_window_resize));
}

bool space_invaders_app::on_window_resize(retro::events::window_resize_event &window_resize_event)
{
    application::on_window_resize(window_resize_event);
    return m_game_manager->on_window_resize(window_resize_event);
}

bool space_invaders_app::on_key_pressed(retro::events::key_pressed_event &key_pressed_event)
{
    return m_game_manager->on_key_pressed(key_pressed_event);
}

retro::core::application *retro::core::create_application()
{
    return new space_invaders_app();
}
