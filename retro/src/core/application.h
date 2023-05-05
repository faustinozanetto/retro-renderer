#pragma once
#include "utils/singleton.h"

// Entry point used in applications.
int main(int argc, char **argv);

namespace retro::core
{
    class application : public singleton<application>
    {
    public:
        application();
        virtual ~application();

    private:
        friend int ::main(int argc, char **argv);
    };

    application *create_application();
}
