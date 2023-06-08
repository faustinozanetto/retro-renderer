#pragma once

// Core Includes
#include "core/application.h"
#include "core/engine_time.h"

// Camera Includes
#include "camera/camera.h"

// Renderer Includes
#include "renderer/window/window.h"
#include "renderer/renderer/renderer.h"

#include "renderer/shaders/shader.h"
#include "renderer/shaders/shader_loader.h"
#include "renderer/shaders/shader_file_watcher.h"

#include "renderer/buffers/vertex_array_object.h"
#include "renderer/buffers/vertex_buffer_object.h"
#include "renderer/buffers/vertex_buffer_layout_descriptor.h"

#include "renderer/textures/texture.h"
#include "renderer/textures/texture_loader.h"

#include "renderer/frame_buffer/frame_buffer.h"
#include "renderer/render_buffer/render_buffer.h"

#include "renderer/materials/material.h"
#include "renderer/materials/material_loader.h"

#include "renderer/models/mesh.h"
#include "renderer/models/model.h"
#include "renderer/models/model_loader.h"

#include "renderer/lighting/light.h"
#include "renderer/lighting/point_light.h"
#include "renderer/lighting/spot_light.h"

#include "renderer/text/font.h"
#include "renderer/text/font_loader.h"
#include "renderer/text/text.h"

// Audio Include
#include "audio/audio_context.h"
#include "audio/sound.h"
#include "audio/sound_emitter.h"
#include "audio/sound_emitter.h"

// Assets Include
#include "assets/asset.h"
#include "assets/asset_pack.h"
#include "assets/asset_manager.h"

// Scene Include
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/actors/scene_actor.h"
#include "scene/actors/components/base_component.h"
#include "scene/actors/components/transform_component.h"

// Input Include
#include "input/input.h"
#include "input/key_codes.h"

// Events Include
#include "events/base_event.h"
#include "events/window_events.h"
#include "events/key_events.h"
#include "events/event_dispatcher.h"

// Interface Includes
#include "interfaces/engine_ui.h"
