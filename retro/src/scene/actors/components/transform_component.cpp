#include "rtpch.h"
#include "transform_component.h"

namespace retro::scene
{
    transform_component::transform_component(const std::shared_ptr<math::transform>& transform)
    {
        m_transform = transform;
    }

	transform_component::transform_component(const glm::vec3& location /*= glm::vec3(0.0f)*/, const glm::quat& rotation /*= glm::quat(1.0f, 0.0f, 0.0, 0.0f)*/, const glm::vec3& scale /*= glm::vec3(1.0f)*/)
	{
        m_transform = std::make_shared<math::transform>(location, rotation, scale);
	}

    transform_component::~transform_component()
    {
    }

    void transform_component::initialize()
    {
    }
}
