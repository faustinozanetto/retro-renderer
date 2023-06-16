#pragma once

#include <string>

namespace retro::editor
{
    class editor_console_message
    {
    public:
        editor_console_message(const std::string &message, const std::string& source) : m_message(message), m_source(source) {}

        /* Getters */
        const std::string& get_message() const { return m_message;}
        const std::string& get_source() const { return m_source;}
    private:
        std::string m_message;
        std::string m_source;
    };
}