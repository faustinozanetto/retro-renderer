#pragma once

namespace retro
{
    class uuid
    {
    public:
        uuid();
        uuid(uint64_t uuid);
        uuid(const uuid &) = default;

        operator uint64_t() const { return m_uuid; }

    private:
        uint64_t m_uuid;
    };
}