#pragma once

#include "retro.h"

class demo_app : public retro::core::application
{
public:
    demo_app();
    ~demo_app() override;

    void on_update() override;
};
