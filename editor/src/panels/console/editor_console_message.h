#pragma once

#include <string>

namespace retro::editor
{
    class editor_console_message
    {
    public:
        editor_console_message(const std::string &message);

        /* Getters */
        const std::string& get_message() const { return m_message; }
    private:
        std::string m_message;
    };
}