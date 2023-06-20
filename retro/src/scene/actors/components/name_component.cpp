#include "rtpch.h"
#include "name_component.h"

namespace retro::scene
{
    name_component::name_component(const std::string &name)
    {
        m_name = name;
    }
    name_component::~name_component()
    {
    }

    void name_component::initialize()
    {
    }
}
