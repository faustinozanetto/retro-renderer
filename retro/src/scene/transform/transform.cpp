#include "rtpch.h"
#include "transform.h"

namespace retro::scene
{
	transform::transform(const glm::vec3& location, const glm::quat& rotation, const glm::vec3& scale)
	{
		m_location = location;
		m_rotation = rotation;
		m_scale = scale;
		m_transform_dirty = false;
		recalculate_transform();
	}

	transform::~transform()
	{
	}

	glm::mat4 transform::get_transform()
	{
		RT_PROFILE_SECTION("transform::get_transform");
		if (!m_transform_dirty) return m_transform;

		recalculate_transform();
		return m_transform;
	}

	void transform::set_location(const glm::vec3& location)
	{
		m_location = location;
		m_transform_dirty = true;
	}

	void transform::set_rotation(const glm::quat& rotation)
	{
		m_rotation = rotation;
		m_transform_dirty = true;
	}

	void transform::set_scale(const glm::vec3& scale)
	{
		m_scale = scale;
		m_transform_dirty = true;
	}

	void transform::recalculate_transform()
	{
		RT_PROFILE_SECTION("transform::recalculate_transform");
		glm::mat4 updated_transform = glm::mat4(1.0f);
		updated_transform = glm::translate(updated_transform, m_location);
		updated_transform *= glm::toMat4(m_rotation);
		updated_transform = glm::scale(updated_transform, m_scale);
		m_transform = updated_transform;
	}
}