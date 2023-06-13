#include "rtpch.h"
#include "physics_error_callback.h"

namespace retro::physics
{
    physics_error_callback::physics_error_callback()
    {
    }

    physics_error_callback::~physics_error_callback()
    {
    }

    void physics_error_callback::reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line)
    {
        RT_ERROR("Retro Renderer | PhysX error reported!");
        RT_ERROR("  - Code: {}", (int)code);
        RT_ERROR("  - Message: {}", message);
        RT_ERROR("  - File: {}", file);
        RT_ERROR("  - Line: {}", line);
    }
}