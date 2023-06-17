#pragma once

#include "core/assert.h"
#include "logging/logger.h"

#include <iostream>

namespace retro
{
    template <class T>
    class singleton
    {
    public:
        static T& get()
        {
            RT_ASSERT_MSG(s_instance, "Singleton instance is invalid!");
            return *s_instance;
        }

        static bool get_initialized() {
            return s_initialized;
        }

        template <typename... TArgs>
        static void initialize(TArgs... args)
        {
            RT_ASSERT_MSG(s_instance == nullptr, "Singleton already initialized!");
            s_instance = new T(std::forward<TArgs>(args)...);
            s_initialized = true;
        }

        static void release()
        {
            if (s_instance)
            {
                delete s_instance;
                s_initialized = false;
                s_instance = nullptr;
            }
        }

    protected:
        singleton()
        = default;

        virtual ~singleton() = default;

        static T* s_instance;
        static bool s_initialized;
    };

    template <class T>
    T* singleton<T>::s_instance = nullptr;

    template <class T>
    bool singleton<T>::s_initialized = false;
}
