#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace retro::math
{
    class transform
    {
    public:
        transform(const glm::vec3 &location = glm::vec3(0.0f), const glm::quat &rotation = glm::quat(1.0f, 0.0f, 0.0, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f));
        ~transform();

        /* Getters */
        const glm::vec3 &get_location() const { return m_location; }
        const glm::quat &get_rotation() const { return m_rotation; }
        const glm::vec3 &get_scale() const { return m_scale; }
        const glm::mat4& get_transform();
        const std::shared_ptr<transform>& get_parent() const { return m_parent; }
		bool has_parent() const { return m_parent != nullptr; }

        /* Setters */
        void set_location(const glm::vec3 &location);
        void set_rotation(const glm::quat &rotation);
        void set_scale(const glm::vec3 &scale);
        void set_parent(const std::shared_ptr<transform>& parent) { m_parent = parent; };

    private:
        void recalculate_transform();

        glm::vec3 m_location;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        std::shared_ptr<transform> m_parent;

        bool m_transform_dirty;
        glm::mat4 m_transform;
    };
}