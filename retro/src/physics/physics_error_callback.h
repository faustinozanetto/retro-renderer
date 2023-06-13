#pragma once

#include "foundation/PxErrorCallback.h"
#include "PxPhysXConfig.h"

namespace retro::physics
{
    class physics_error_callback : public physx::PxErrorCallback
    {
    public:
        physics_error_callback();
        virtual ~physics_error_callback();

        virtual void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override;
    };
}
